#include "Framework.h"
#include "DungeonTileMap.h"
#include "DungeonBgTile.h"
#include "BgTileManager.h"

DungeonTileMap::DungeonTileMap(string file)
{
	tileSize = { 80.0f, 80.0f };
	Pos() = tileSize;
	Load(file);
	UpdateWorld();

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            SetGrid(x, y);
}

DungeonTileMap::~DungeonTileMap()
{
}

void DungeonTileMap::SetGrid(int x, int y)
{
    //유효하지 않은 위치
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
        //주변타일 좌표 유효성 검사
        if (check.first >= width || check.first < 0)
            continue;
        if (check.second >= height || check.second < 0)
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
	bgTiles.resize(size);

	UINT i = 0;
	for (auto& tile : bgTiles) {
		Tile::Data data;
		data.textureFile = reader->WString();

		data.pos.x = tileSize.x * (i % width);
		reader->Float();
		data.pos.y = tileSize.x * (i / width);
		reader->Float();
		
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		tile = new DungeonBgTile(data, tileSize);
		tile->SetParent(this);
		tile->UpdateWorld();
		i++;
	}

	//맵 타일의 경로를 인식해 매니저에 타일 종류를 인식시킨다
	wstring path = bgTiles[0]->GetTexture()->GetFile();
	auto p = path.find_last_of(L"/");
	path = path.substr(0, p+1);
	BgTileManager::Get()->SetTexture(path);

	size = reader->UInt();
	objTiles.resize(size);
	for (auto& tile : objTiles) {
		Tile::Data data;
		data.textureFile = reader->WString();
		data.pos.x = reader->Float();
		data.pos.y = reader->Float();
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		tile = new Tile(data);
		tile->SetParent(this);
		tile->Update();
	}

	delete reader;
}
