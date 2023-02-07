#include "Framework.h"

EditTileMap::EditTileMap(UINT width, UINT height)
	: width(width), height(height)
{


	CreateTile();

	Vector2 rightTop(width * tileSize.x, height * tileSize.y);
	rightTop += {80.0f, 80.0f};

	CAM->SetRightTop(rightTop + tileSize * 10);
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
	clickedPos = CAM->ScreenToWorld(mousePos);

	for (auto tile : bgTiles)
		tile->Update();

	for (auto tile : objTiles)
		tile->Update();


	UpdateWorld();
}

void EditTileMap::Render()
{

	for (auto tile : bgTiles) {
		if (!CAM->ContainFrustum(tile->GlobalPos(), tile->GetSize() * tile->GetSize()))
			continue;

		tile->Render();
		tile->GetCollider()->Render();
	}

	for (auto tile : objTiles) {
		if (!CAM->ContainFrustum(tile->GlobalPos(), tile->GetSize() * tile->GetSize()))
			continue;

		tile->Render();
		tile->GetCollider()->Render();
	}
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
	UINT i = 0;
	for (Tile* tile : bgTiles) {
		Tile::Data data = tile->GetData();

		writer->WString(data.textureFile);
		writer->UInt(i % width);
		writer->UInt(i / width);
//		writer->Float(data.pos.x);
//		writer->Float(data.pos.y);
		writer->Float(data.angle);
		writer->Int(data.type);

		i++;
	}

	writer->UInt(objTiles.size());

	for (auto tile : objTiles) {
		Tile::Data data = tile->GetData();
		if (data.textureFile.empty())
			continue;

		writer->WString(data.textureFile);
		//writer->UInt(i % width);
		//writer->UInt(i / width);
		writer->UInt((UINT)floorf(data.pos.x / tileSize.x));
		writer->UInt((UINT)floorf(data.pos.y / tileSize.y));
		writer->Float(data.angle);
		writer->Int(data.type);

		i++;
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
		data.pos.x = reader->UInt() * tileSize.x;
		data.pos.y = reader->UInt() * tileSize.y;
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
		data.pos.x = reader->UInt() * tileSize.x;
		data.pos.y = reader->UInt() * tileSize.y;
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		tile = new Tile(data, tileSize);
		tile->SetParent(this);
	}

	delete reader;
}

void EditTileMap::SetBGUV(Vector2 startUV, Vector2 endUV)
{
	for (auto tile : bgTiles) {
		tile->ModifyUV(startUV, endUV);
	}
}

void EditTileMap::CreateTile()
{
	wstring baseTile = L"Textures/Tile/tile.png";
	Texture* texture = Texture::Add(baseTile);

	//tileSize = texture->GetSize();

	bgTiles.reserve(width * height);

	for (UINT y = 0; y < height; y++) {
		for (UINT x = 0; x < width; x++) {
			Tile::Data data = {};
			data.textureFile = baseTile;
			data.pos = { tileSize.x * x, tileSize.y * y };
			Tile* tile = new Tile(data, tileSize);
			tile->SetParent(this);
			bgTiles.push_back(tile);
		}
	}
}


void EditTileMap::SetBGTile(wstring file, float angle)
{
	for (Tile* tile : bgTiles)
	{
		if (tile->GetCollider()->IsPointCollision(clickedPos))
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
		if (tile->GetCollider()->IsPointCollision(clickedPos))
		{
			Tile::Data data = tile->GetData();
			data.textureFile = file;
			data.angle = angle;
			data.type = Tile::OBJ;

			Tile* obj = new Tile(data, tile->GetSize());
			obj->SetParent(this);
			obj->Pos() = tile->Pos();
			objTiles.push_back(obj);
		}
	}
}
