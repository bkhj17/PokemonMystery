#include "Framework.h"
#include "DungeonTileMap.h"
#include "DungeonBgTile.h"
#include "BgTileManager.h"
#include "DungeonObjTile.h"
#include "DungeonAStar.h"
#include "ObjTileManager.h"
#include "../Data/DungeonDataManager.h"

DungeonTileMap::DungeonTileMap()
{
	ObjTileManager::Get();

	tileSize = { 100, 100 };
	Pos() = tileSize * 0.5f;
	
	Vector2 maxFrame = BgTileManager::Get()->GRID_SIZE;
	quad = new Quad(tileSize);
	quad->ModifyUV(Vector2(), Vector2(1, 1) / maxFrame);

	quad->SetVertexShader(L"DungeonTile.hlsl");
	quad->SetPixelShader(L"DungeonTile.hlsl");

	instances.resize(MAX_WIDTH * MAX_HEIGHT);	
	for (auto& instance : instances)
		instance.maxFrame = maxFrame;
	
	instanceBuffer = new VertexBuffer(instances.data(), sizeof(TileInstanceData), MAX_WIDTH * MAX_HEIGHT);
	
	astar = new DungeonAStar(this);
}

DungeonTileMap::~DungeonTileMap()
{
	delete instanceBuffer;
	delete quad;
	delete astar;
	ObjTileManager::Delete();
}

void DungeonTileMap::Init(string key, int floorNum)
{
	floorData = DungeonDataManager::Get()->GetData(key, floorNum);
	if (!floorData)
		return;

	Load(floorData->file);

	for (UINT y = 0; y < height; y++)
		for (UINT x = 0; x < width; x++)
			SetGrid(x, y);

	SetUpTrap();
	SetUpPlayerStart();

	UpdateWorld();

	CAM->SetLeftBottom(LeftBottom());
	CAM->SetRightTop(RightTop());
}

void DungeonTileMap::UpdateWorld()
{
	__super::UpdateWorld();
	//카메라 움직일 때만 해야 함. 안 그러면 의미 없음
	for (int i = 0; i < bgTiles.size(); i++) {
		auto tile = (DungeonBgTile*)bgTiles[i];
		tile->GetGridFlag();

		instances[i].transform = DirectX::XMMatrixTranspose(tile->GetWorld());
		instances[i].curFrame = BgTileManager::Get()->GetGrid(tile->GetGridFlag());

		UINT type = BgTileManager::Get()->GetTileType(bgTiles[i]->GetTexture());
		instances[i].tileType = (int)type;
	}
	instanceBuffer->Update(instances.data(), (int)instances.size());
}

void DungeonTileMap::Render()
{
	//배경 타일에는 인스턴싱 사용
	instanceBuffer->Set(1);
	quad->SetRender();
	BgTileManager::Get()->GetLandTexture()->PSSet(1);
	BgTileManager::Get()->GetWaterTexture()->PSSet(2);
	DC->DrawIndexedInstanced(6, (UINT)bgTiles.size(), 0, 0, 0);

	//
	for (auto obj : objTiles)
		obj->Render();
}

void DungeonTileMap::GetNodes(vector<Node*>& nodes)
{
	for (auto tile : bgTiles) {
		Vector2 tilePos = tile->GlobalPos();
		Node* node = new Node(tilePos, (int)nodes.size());

		nodes.push_back(node);
	}
}

POINT DungeonTileMap::PosToPoint(Vector2 pos)
{
	POINT result = { +0, +0 };
	Vector2 p = pos / tileSize;
	
	result = { (LONG)p.x, (LONG)p.y };

	return result;
}

bool DungeonTileMap::SetMove(IN int startX, IN int startY, IN int dirX, IN int dirY, OUT Vector2& destPos)
{
	auto curTile = (DungeonBgTile*)bgTiles[startY * width + startX]; 
	int flag = curTile->GetGridFlag();

	bool result = BgTileManager::Get()->CheckMovable(flag, dirX, dirY);

	if (result) {
		destPos = { tileSize.x * dirX, tileSize.y * dirY };
		destPos += curTile->GlobalPos();
	}
	return result;
}

vector<POINT> DungeonTileMap::GetPointsByCondition(function<bool(POINT)> condition)
{
	vector<POINT> points;
	for (int i = 0; i < bgTiles.size(); i++) {
		POINT tilePoint = { i % (LONG)width, i / (LONG)width };
		//애초에 벽
		if (condition(tilePoint))
			points.push_back(tilePoint);
	}

	return points;
}

POINT DungeonTileMap::GetRandomPointByCondition(function<bool(POINT)> condition)
{
	vector<POINT> roomPoints = GetPointsByCondition(condition);
	return roomPoints[Random(0, (int)roomPoints.size())];
}

vector<pair<int, int>> DungeonTileMap::DetectableTiles(POINT curPoint)
{
	map<pair<int, int>, int> check;
	//3 : 플레이어에 의한 주변 확인
	//2 : 방 타일에 의한 주변 확인
	//1 : 길 타일에 의한 주변 확인
	priority_queue<DetectNode> pq;

	pq.push({ {curPoint.x, curPoint.y}, 3, 0 });
	POINT dir[8] = {
		{-1, 1}, //leftUp
		{0, 1}, //up
		{1, 1}, //rightUp
		{-1, 0}, //left
		{1, 0}, //right
		{-1, -1}, //leftDown
		{0, -1}, //down
		{1, -1}, //rightDown
	};

	while (!pq.empty()) {
		DetectNode curNode = pq.top();
		pq.pop();

		if (check.find(curNode.point) != check.end() && check[curNode.point] >= curNode.flag)
			continue;
		check[curNode.point] = curNode.flag;

		int gridFlag = ((DungeonBgTile*)bgTiles[curNode.point.second * width + curNode.point.first])->GetGridFlag();

		int nextFlag = 1;
		bool isRoom = BgTileManager::Get()->IsRoom(gridFlag);
		if (curNode.flag == 3 || (curNode.flag == 2 && isRoom))
			nextFlag = 2;
		else if (curNode.flag == 1)
			nextFlag = 0;

		if (curNode.flag >= 1) {
			for (int i = 0; i < 8; i++) {
				Vector2 dest;
				if (!SetMove(curNode.point.first, curNode.point.second, dir[i].x, dir[i].y, dest))
					continue;

				DetectNode nextNode;
				nextNode.point = { curNode.point.first + dir[i].x, curNode.point.second + dir[i].y };
				nextNode.flag = nextFlag;
				nextNode.dist = curNode.dist + 1;

				pq.push(nextNode);
			}
		}
	}

	vector<pair<int, int>> result;
	for (pair<pair<int, int>, int> const& point : check) {
		pair<int, int> p = point.first;
		result.emplace_back(p); //왜 안 들어가?
	}
	return result;
}

DungeonBgTile* DungeonTileMap::GetBgTile(POINT point)
{
	if(point.x >= (int)width || point.x < 0 || point.y >= (int)height || point.y)
		return nullptr;

	return (DungeonBgTile*)bgTiles[point.y * width + point.x];
}

void DungeonTileMap::SetGrid(int x, int y)
{
    //유효하지 않은 위치
    if (x < 0 || x >= (int)width || y < 0 || y >= (int)height)
        return;

    auto targetTile = (DungeonBgTile*)bgTiles[(size_t)y * width + x];

    pair<int, int> dir[8] = {
        {-1, 1}, //leftUp
        {0, 1}, //up
        {1, 1}, //rightUp
        {-1, 0}, //left
        {1, 0}, //right
        {-1, -1}, //leftDown
        {0, -1}, //down
        {1, -1}, //rightDown
    };

    int flag = 0;

    for (int i = 0; i < 8; i++) {
        pair<int, int> check = { x + dir[i].first, y + dir[i].second };
        //주변타일 좌표 유효성 검사
        if (check.first >= (int)width || check.first < 0)
            continue;
        if (check.second >= (int)height || check.second < 0)
            continue;

        auto nearTile = bgTiles[check.second * width + check.first];
        //두 타일의 텍스처가 같다면 플래그 on
        if (targetTile->GetTexture() == nearTile->GetTexture())
            flag |= 1 << i;
    }

    targetTile->UpdateGrid(flag);
}

void DungeonTileMap::Load(string file)
{
	BinaryReader* reader = new BinaryReader(file);
	if (reader->IsFailed())
		return;

	width = reader->UInt();
	height = reader->UInt();

	UINT size = reader->UInt();

	while (size < bgTiles.size()) {
		delete bgTiles.back();
		bgTiles.pop_back();
	}
	bgTiles.resize(size);

	for (auto& tile : bgTiles) {
		Tile::Data data;
		data.textureFile = reader->WString();
		data.pos.x = reader->UInt() * tileSize.x;
		data.pos.y = reader->UInt() * tileSize.y;
		
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		if (!tile) {
			tile = new DungeonBgTile(data, tileSize);
			tile->SetParent(this);
			tile->UpdateWorld();
		}
		else {
			auto bgTile = ((DungeonBgTile*)tile);
			bgTile->SetData(data);
		}
	}

	//맵 타일의 경로를 인식해 매니저에 타일 종류를 인식시킨다
	wstring path = bgTiles[0]->GetTexture()->GetFile();
	auto p = path.find_last_of(L"/");
	path = path.substr(0, p+1);
	BgTileManager::Get()->SetTexture(path);
	quad->SetTexture(BgTileManager::Get()->GetWallTexture()->GetFile());

	//오브젝트 타일은 삭제로 초기화 - 이쪽이 깔끔하다 생각했다
	ObjTileManager::Get()->Clear();
	for (auto tile : objTiles) {
		if(tile != nullptr)
			delete tile;
	}
	objTiles.clear();

	//고정 오브젝트 설정
	size = reader->UInt();
	objTiles.reserve(size);
	for (UINT i = 0; i < size; i++) {
		Tile::Data data;
		data.textureFile = reader->WString();
		if (data.textureFile.empty())
			continue;

		UINT x = reader->UInt(), y = reader->UInt();
		data.pos = tileSize * Vector2((float)x, (float)y);
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		Tile* tile = new DungeonObjTile("Stair", data, tileSize);
		tile->SetParent(this);
		tile->UpdateWorld();
		objTiles.push_back(tile);
		ObjTileManager::Get()->Register(x, y, (DungeonObjTile*)tile);
	}
	objTiles.shrink_to_fit();
	delete reader;
}

void DungeonTileMap::SetUpTrap()
{
	if (floorData->maxTrapNum == 0)
		return;
	
	int trapNum = Random(1, floorData->maxTrapNum);

	vector<POINT> roomPoints = GetPointsByCondition([this](POINT point) -> bool {
		//잘못된 좌표다
		if (point.y * width + point.x >= bgTiles.size()) return false;
		auto tile = (DungeonBgTile*)bgTiles[point.y * width + point.x];
		//벽 혹은 물이다
		if (tile->GetTexture() != BgTileManager::Get()->GetLandTexture()) return false;
		//다른 타일이 있다
		if (ObjTileManager::Get()->GetTile(point.x, point.y)) return false;
		//방이다
		return BgTileManager::Get()->IsRoom(tile->GetGridFlag());
	});
	//셔플
	if ((int)roomPoints.size() <= trapNum) {
		trapNum = (int)roomPoints.size();
	}
	else {
		for (int i = 0; i < trapNum; i++) {
			int j = Random(i + 1, trapNum);
			swap(roomPoints[i], roomPoints[j]);
		}
	}
	//오브젝트 타일 생성 및 배치
	for (int i = 0; i < trapNum; i++) {
		string trapKey = floorData->traps[Random(0, (int)floorData->traps.size())];

		Tile::Data data;

		//유효하지 않은 키
		data.textureFile = ObjTileManager::Get()->GetTextureFile(trapKey);
		if(data.textureFile.empty())
			continue;

		data.pos = { roomPoints[i].x * tileSize.x, roomPoints[i].y * tileSize.y };
		data.angle = 0.0f;
		data.type = Tile::OBJ;

		DungeonObjTile* objTile = new DungeonObjTile(trapKey, data, tileSize);
		objTile->SetParent(this);
		objTiles.push_back(objTile);
		auto post = ObjTileManager::Get()->Register(roomPoints[i].x, roomPoints[i].y, objTile);
		if (post != nullptr) {
			//그럴 일은 없겠지만 만약 위치가 중복되었다면 원래 있던걸 없애버린다 
			auto it = find(objTiles.begin(), objTiles.end(), post);
			if (it != objTiles.end())
				objTiles.erase(it);
			delete post;
		}
	}
}

void DungeonTileMap::SetUpPlayerStart()
{
	vector<POINT> roomPoints = GetPointsByCondition([this](POINT point) -> bool {
		//잘못된 좌표다
		if (point.y * width + point.x >= bgTiles.size()) return false;
		auto tile = (DungeonBgTile*)bgTiles[point.y * width + point.x];
		//벽 혹은 물이다
		if (tile->GetTexture() != BgTileManager::Get()->GetLandTexture()) return false;
		//다른 타일이 있다
		if (ObjTileManager::Get()->GetTile(point.x, point.y)) return false;
		//방이다
		return BgTileManager::Get()->IsRoom(tile->GetGridFlag());
	});

	playerStartPoint = roomPoints[Random(0, (int)roomPoints.size())];
}
