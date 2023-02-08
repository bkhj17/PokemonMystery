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
}

void DungeonTileMap::Init(string key, int floorNum)
{
	floorData = DungeonDataManager::Get()->GetData(key, floorNum);
	if (!floorData)
		return;

	Load(floorData->file);

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
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
	//ī�޶� ������ ���� �ؾ� ��. �� �׷��� �ǹ� ����
	for (int i = 0; i < bgTiles.size(); i++) {
		auto tile = (DungeonBgTile*)bgTiles[i];
		tile->GetGridFlag();

		instances[i].transform = DirectX::XMMatrixTranspose(tile->GetWorld());
		instances[i].curFrame = BgTileManager::Get()->GetGrid(tile->GetGridFlag());

		UINT type = BgTileManager::Get()->GetTileType(bgTiles[i]->GetTexture());
		instances[i].tileType = (int)type;
	}
	instanceBuffer->Update(instances.data(), instances.size());
}

void DungeonTileMap::Render()
{
	//��� Ÿ�Ͽ��� �ν��Ͻ� ���
	instanceBuffer->Set(1);
	quad->SetRender();
	BgTileManager::Get()->GetLandTexture()->PSSet(1);
	BgTileManager::Get()->GetWaterTexture()->PSSet(2);
	DC->DrawIndexedInstanced(6, bgTiles.size(), 0, 0, 0);

	//
	for (auto obj : objTiles)
		obj->Render();
}

void DungeonTileMap::GetNodes(vector<Node*>& nodes)
{
	for (auto tile : bgTiles) {
		Vector2 tilePos = tile->GlobalPos();
		Node* node = new Node(tilePos, nodes.size());

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
		//���ʿ� ��
		if (condition(tilePoint))
			points.push_back(tilePoint);
	}

	return points;
}

POINT DungeonTileMap::GetRandomPointByCondition(function<bool(POINT)> condition)
{
	vector<POINT> roomPoints = GetPointsByCondition(condition);
	return roomPoints[Random(0, roomPoints.size())];
}

void DungeonTileMap::SetGrid(int x, int y)
{
    //��ȿ���� ���� ��ġ
    if (x < 0 || x >= width || y < 0 || y >= height)
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
        //�ֺ�Ÿ�� ��ǥ ��ȿ�� �˻�
        if (check.first >= width || check.first < 0)
            continue;
        if (check.second >= height || check.second < 0)
            continue;

        auto nearTile = bgTiles[check.second * width + check.first];
        //�� Ÿ���� �ؽ�ó�� ���ٸ� �÷��� on
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

	//�� Ÿ���� ��θ� �ν��� �Ŵ����� Ÿ�� ������ �νĽ�Ų��
	wstring path = bgTiles[0]->GetTexture()->GetFile();
	auto p = path.find_last_of(L"/");
	path = path.substr(0, p+1);
	BgTileManager::Get()->SetTexture(path);
	quad->SetTexture(BgTileManager::Get()->GetWallTexture()->GetFile());

	//������Ʈ Ÿ���� ������ �ʱ�ȭ - ������ ����ϴ� �����ߴ�
	ObjTileManager::Get()->Clear();
	for (auto tile : objTiles) {
		if(tile != nullptr)
			delete tile;
	}
	objTiles.clear();

	//���� ������Ʈ ����
	size = reader->UInt();
	objTiles.reserve(size);
	for (int i = 0; i < size; i++) {
		Tile::Data data;
		data.textureFile = reader->WString();
		if (data.textureFile.empty())
			continue;

		UINT x = reader->UInt(), y = reader->UInt();
		data.pos = tileSize * Vector2(x, y);
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
		//�߸��� ��ǥ��
		if (point.y * width + point.x >= bgTiles.size()) return false;
		auto tile = (DungeonBgTile*)bgTiles[point.y * width + point.x];
		//�� Ȥ�� ���̴�
		if (tile->GetTexture() != BgTileManager::Get()->GetLandTexture()) return false;
		//�ٸ� Ÿ���� �ִ�
		if (ObjTileManager::Get()->GetTile(point.x, point.y)) return false;
		//���̴�
		return BgTileManager::Get()->IsRoom(tile->GetGridFlag());
	});
	//����
	if (roomPoints.size() <= trapNum) {
		trapNum = roomPoints.size();
	}
	else {
		for (int i = 0; i < trapNum; i++) {
			int j = Random(i + 1, trapNum);
			swap(roomPoints[i], roomPoints[j]);
		}
	}
	//������Ʈ Ÿ�� ���� �� ��ġ
	for (int i = 0; i < trapNum; i++) {
		string trapKey = floorData->traps[Random(0, floorData->traps.size())];

		Tile::Data data;

		//��ȿ���� ���� Ű
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
			//�׷� ���� �������� ���� ��ġ�� �ߺ��Ǿ��ٸ� ���� �ִ��� ���ֹ����� 
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
		//�߸��� ��ǥ��
		if (point.y * width + point.x >= bgTiles.size()) return false;
		auto tile = (DungeonBgTile*)bgTiles[point.y * width + point.x];
		//�� Ȥ�� ���̴�
		if (tile->GetTexture() != BgTileManager::Get()->GetLandTexture()) return false;
		//�ٸ� Ÿ���� �ִ�
		if (ObjTileManager::Get()->GetTile(point.x, point.y)) return false;
		//���̴�
		return BgTileManager::Get()->IsRoom(tile->GetGridFlag());
	});

	playerStartPoint = roomPoints[Random(0, roomPoints.size())];
}
