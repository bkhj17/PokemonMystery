#include "Framework.h"
#include "TileMap0126.h"
#include "EnemyTank0126.h"

TileMap0126::TileMap0126(string file) 
	: GameTileMap(file) 
{
	SetUp(); 
}

void TileMap0126::BulletCollision()
{
	for (auto obj : breakables)
		BulletManager::Get()->CheckCollision(obj->GetCollider(), bind(&Tile::SetActive, obj, false));

	for (auto obj : unbreakables)
		BulletManager::Get()->CheckCollision(obj->GetCollider(), nullptr);

	for (auto obj : protects)
		BulletManager::Get()->CheckCollision(obj->GetCollider(), bind(&TileMap0126::BreakProtect, this, obj));
}

void TileMap0126::PushObject(Collider* collider)
{
	for (auto obj : breakables) {
		if(obj->Active())
			obj->GetCollider()->PushCollider(collider);
	}

	for (auto obj : unbreakables)
		obj->GetCollider()->PushCollider(collider);
}

void TileMap0126::Render()
{
	for (auto bg : bgTiles)
		bg->Render();

	for (auto obj : objTiles) {
		if (obj->Active())
			obj->Render();
	}
}

Vector2 TileMap0126::EnemySpawnPoint()
{
	if (enemySpawns.empty())
		return { -1.0f, -1.0f };

	int p = Random(0, (int)enemySpawns.size());
	return enemySpawns[p]->GlobalPos();
}

bool TileMap0126::PointCollision(Vector2 pos)
{
	for (auto obj : breakables) {
		if (obj->GetCollider()->IsPointCollision(pos))
			return true;
	}

	for (auto obj : unbreakables) {
		if (obj->GetCollider()->IsPointCollision(pos))
			return true;
	}

	return false;
}

void TileMap0126::SetUp()
{
	collideTable[L"Textures/Tile/barrel.png"] = SPAWN_POINT;
	collideTable[L"Textures/Tile/crate.png"] = BREAKABLE;
	collideTable[L"Textures/Tile/wall.png"] = UNBREAKABLE;
	collideTable[L"Textures/Tile/wall_corner.png"] = UNBREAKABLE;
	collideTable[L"Textures/Tile/campfire.png"] = PROTECT;
	collideTable[L"Textures/Tile/plants.png"] = PLAYER_SPAWN;

	for (auto obj : objTiles) {
		wstring fileName = obj->GetTexture()->GetFile();

		if (collideTable.find(fileName) == collideTable.end())
			continue;

		switch (collideTable[fileName])
		{
		case TileMap0126::SPAWN_POINT:
			enemySpawns.push_back(obj);
			break;
		case TileMap0126::BREAKABLE:
			breakables.push_back(obj);
			break;
		case TileMap0126::UNBREAKABLE:
			unbreakables.push_back(obj);
			break;
		case PROTECT:
			protects.push_back(obj);
			break;
		case PLAYER_SPAWN:
			playerSpawn = obj->Pos();
		}
	}

	protectNum = protects.size();
}

void TileMap0126::BreakProtect(Tile* protect)
{
	protect->SetActive(false);
	protectNum--;

	if (loseEvent)
		loseEvent();
}
