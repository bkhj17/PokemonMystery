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

void EditTileMap::DeleteObjTile()
{
	/*
	for (UINT i = 0; i < objTiles.size(); i++) {
		if (objTiles[i]->GetCollider()->IsPointCollision(mousePos)) {
			delete objTiles[i];
			objTiles.erase(objTiles.begin() + i);
			return;
		}
	}
	*/
	
	for (auto it = objTiles.begin(); it != objTiles.end();) {
		if ((*it)->GetCollider()->IsPointCollision(mousePos)) {
			delete* it;
			it = objTiles.erase(it);
		}
		else
			it++;
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

	for (auto tile : objTiles)
		delete tile;

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
	}

	delete reader;
}

void EditTileMap::CreateTile()
{
	wstring baseTile = L"Textures/Tile/tile.png";
	Texture* texture = Texture::Add(baseTile);

	tileSize = texture->GetSize();

	bgTiles.reserve(width * height);

	for (UINT y = 0; y < height; y++) {
		for (UINT x = 0; x < width; x++) {
			Tile::Data data = {};
			data.textureFile = baseTile;
			data.pos = { tileSize.x * x, tileSize.y * y };
			Tile* tile = new Tile(data);
			tile->SetParent(this);
			bgTiles.push_back(tile);
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
	DeleteObjTile();

	for (Tile* tile : bgTiles)
	{
		if (tile->GetCollider()->IsPointCollision(mousePos))
		{
			Tile::Data data = tile->GetData();
			data.textureFile = file;
			data.angle = angle;
			data.type = Tile::OBJ;

			Tile* obj = new Tile(data);
			obj->SetParent(this);
			obj->Pos() = tile->Pos();
			objTiles.push_back(obj);
		}
	}

	/*
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
	*/
}
