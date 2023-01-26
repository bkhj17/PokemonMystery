#include "Framework.h"

EditTileMap::EditTileMap(UINT width, UINT height)
	: width(width), height(height)
{
	CreateTile();
}

EditTileMap::~EditTileMap()
{
	for (auto tile : bgTiles)
		delete tile;

	for (auto tile : objTiles)
		delete tile;
}

void EditTileMap::Update()
{
	for (auto tile : bgTiles)
		tile->Update();

	for (auto tile : objTiles)
		tile->Update();

	UpdateWorld();
}

void EditTileMap::Render()
{
	for (auto tile : bgTiles)
		tile->Render();

	for (auto tile : objTiles)
		tile->Render();
}

void EditTileMap::ClickTile(Quad* selectSample, Tile::Type type)
{
	switch (type)
	{
	case Tile::BG:
		SetBGTile(selectSample->GetTexture()->GetFile(), selectSample->Rot().z);
		break;
	case Tile::OBJ:
		SetOBJTile(selectSample->GetTexture()->GetFile(), selectSample->Rot().z);
		break;
	}
}

void EditTileMap::Save(string file)
{
	BinaryWriter* writer = new BinaryWriter(file);

	writer->UInt(width);
	writer->UInt(height);

	writer->UInt(bgTiles.size());
	for (Tile* tile : bgTiles) {
		Tile::Data data = tile->GetData();

		writer->WString(data.textureFile);
		writer->Float(data.pos.x);
		writer->Float(data.pos.y);
		writer->Float(data.angle);
		writer->Int(data.type);
	}

	writer->UInt(objTiles.size());
	for (auto tile : objTiles) {
		Tile::Data data = tile->GetData();
		writer->Int(tile->Active());
		writer->WString(data.textureFile);
		writer->Float(data.pos.x);
		writer->Float(data.pos.y);
		writer->Float(data.angle);
		writer->Int(data.type);
	}

	delete writer;
}

void EditTileMap::Load(string file)
{
	BinaryReader* reader = new BinaryReader(file);

	width = reader->UInt();
	height = reader->UInt();

	UINT size = reader->UInt();

	for (auto tile : bgTiles) {
		Tile::Data data;
		data.textureFile = reader->WString();
		data.pos.x = reader->Float();
		data.pos.y = reader->Float();
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		tile->SetTexture(data.textureFile);
		tile->Pos() = data.pos;
		tile->Rot().z = data.angle;
	}

	size = reader->UInt();
	for (auto tile : objTiles) {
		Tile::Data data;
		bool active = reader->Int();
		data.textureFile = reader->WString();
		data.pos.x = reader->Float();
		data.pos.y = reader->Float();
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		tile->SetActive(active);
		tile->SetTexture(data.textureFile);
		tile->Pos() = data.pos;
		tile->Rot().z = data.angle;
	}

	delete reader;
}

void EditTileMap::CreateTile()
{
	wstring baseTile = L"Textures/Tile/tile.png";
	Texture* texture = Texture::Add(baseTile);

	tileSize = texture->GetSize();

	bgTiles.reserve(width * height);
	objTiles.reserve(width * height);

	for (UINT y = 0; y < height; y++) {
		for (UINT x = 0; x < width; x++) {
			Tile::Data data = {};
			data.textureFile = baseTile;
			data.pos = { tileSize.x * x, tileSize.y * y };
			Tile* tile = new Tile(data);
			tile->SetParent(this);
			bgTiles.push_back(tile);

			data.type = Tile::OBJ;
			tile = new Tile(data);
			tile->SetParent(this);
			tile->SetActive(false);
			objTiles.push_back(tile);
		}
	}
}


void EditTileMap::SetBGTile(wstring file, float angle)
{
	for (Tile* tile : bgTiles)
	{
		if (tile->GetCollider()->IsPointCollision(mousePos))
		{
			tile->SetTexture(file);
			tile->SetAngle(angle);
		}
	}
}

void EditTileMap::SetOBJTile(wstring file, float angle)
{
	int i = 0;
	for (; i < bgTiles.size(); i++) {
		if (bgTiles[i]->GetCollider()->IsPointCollision(mousePos))
			break;
	}

	if (i == bgTiles.size())
		return;

	objTiles[i]->SetActive(true);
	objTiles[i]->SetTexture(file);
	objTiles[i]->SetAngle(angle);
}
