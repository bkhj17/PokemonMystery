#include "Framework.h"
#include "EnemyTank0126.h"
#include "TileMap0126.h"

EnemyTank0126::EnemyTank0126()
{
	isActive = false;
	shooterTag = "EnemyTank";
	shotTime = shotRate;

	colorBuffer->Get() = RED;
}

EnemyTank0126::~EnemyTank0126()
{
}

void EnemyTank0126::Update()
{
	if (!isActive)
		return;

	__super::Update();

	Fire();
}

void EnemyTank0126::Control()
{
	Vector2 forward = GlobalPos() + Right() * 50.0f;

	if (moveDist < moveRate) {
		Vector2 dir = Right() * moveSpeed * DELTA;
		moveDist += dir.Length();
		Pos() += dir;
		return;
	}

	if (tileMap != nullptr && tileMap->PointCollision(forward)) {
		Vector2 left = GlobalPos() + Up() * 50.0f;
		if (!tileMap->PointCollision(left)) {
			Rot().z += XM_PIDIV2;
			return;
		}
		Rot().z -= XM_PIDIV2;
	} else 
		moveDist = 0.0f;
}

void EnemyTank0126::Fire()
{
	if (shotTime >= 0.0f)
		shotTime -= DELTA;

	if (shotTime < 0.0f) {
		__super::Fire();
		shotTime = shotRate;
	}
}

void EnemyTank0126::Spawn(Vector2 pos)
{
	SetActive(true);
	Pos() = pos;
	Rot().z = -XM_PIDIV2;
	moveDist = 0.0f;
	shotTime = 0.0f;
	UpdateWorld();
}

void EnemyTank0126::SetTileMap(TileMap0126* tileMap)
{
	this->tileMap = tileMap;
	moveRate = tileMap->GetTileSize().y;

}