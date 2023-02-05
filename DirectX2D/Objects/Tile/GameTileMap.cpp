#include "Framework.h"

GameTileMap::GameTileMap(string file)
{
	UpdateWorld();

	Load(file);
}

GameTileMap::~GameTileMap()
{
	for (auto tile : bgTiles)
		delete tile;

	for (auto tile : objTiles)
		delete tile;
}

void GameTileMap::UpdateWorld()
{
	__super::UpdateWorld();
	for (auto tile : bgTiles)
		tile->UpdateWorld();
	for (auto tile : objTiles)
		tile->UpdateWorld();
}

void GameTileMap::Render()
{
	for (auto tile : bgTiles)	
		tile->Render();

	for (auto tile : objTiles)
		tile->Render();
}

void GameTileMap::PushObject(Collider* collider)
{
	for (auto tile : objTiles)
		tile->GetCollider()->PushCollider(collider);
}

void GameTileMap::PushRect(RectCollider* collider)
{
	Vector2 overlap;
	for (Tile* tile : objTiles)
		tile->PushRect(collider);
}

void GameTileMap::GetNodes(vector<Node*>& nodes)
{
	for (auto tile : bgTiles) {
		Vector2 tilePos = tile->GlobalPos();
		Node* node = new Node(tilePos, nodes.size());

		for (Tile* obj : objTiles) {
			Vector2 objPos = obj->GlobalPos();

			if (objPos == tilePos)
				node->SetState(Node::OBSTACLE);
		}

		nodes.push_back(node);
	}
}

Vector2 GameTileMap::PointToPos(POINT point)
{
	return GlobalPos() + tileSize * point;
}


Vector2 GameTileMap::LeftBottom()
{
	return Pos() - tileSize * 0.5f;
}

Vector2 GameTileMap::RightTop()
{
	return LeftBottom() + Vector2(width, height) * tileSize;
}

void GameTileMap::Load(string file)
{
	BinaryReader* reader = new BinaryReader(file);
	if (reader->IsFailed())
		return;

	width = reader->UInt();
	height = reader->UInt();

	UINT size = reader->UInt();
	bgTiles.resize(size);

	for (auto& tile : bgTiles) {
		Tile::Data data;
		data.textureFile = reader->WString();
//		data.pos.x = reader->Float();
//		data.pos.y = reader->Float();
		data.pos.x = reader->UInt() * tileSize.x;
		data.pos.y = reader->UInt() * tileSize.y;
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		tile = new Quad(data.textureFile);
		tile->Pos() = data.pos * tileSize;
		tile->Rot().z = data.angle;
		tile->SetParent(this);
		tile->UpdateWorld();
	}

	size = reader->UInt();
	objTiles.resize(size);
	for (auto& tile : objTiles) {
		Tile::Data data;
		data.textureFile = reader->WString();
//		data.pos.x = reader->Float();
//		data.pos.y = reader->Float();
		data.pos.x = reader->UInt() * tileSize.x;
		data.pos.y = reader->UInt() * tileSize.y;
		data.angle = reader->Float();
		data.type = (Tile::Type)reader->Int();

		tile = new Tile(data);
		tile->SetParent(this);
		tile->Update();
	}

	delete reader;
}
