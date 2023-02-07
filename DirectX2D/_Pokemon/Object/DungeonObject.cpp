#include "Framework.h"
#include "DungeonObject.h"
#include "../Unit/UnitMovement.h"
#include "../Tile/DungeonTileMap.h"

DungeonObject::DungeonObject(Vector2 size)
{
	this->size = size;

	collider = new RectCollider(size);
	collider->SetParent(this);

	movement = new UnitMovement;
	movement->SetOwner(this);
}

DungeonObject::~DungeonObject()
{
	delete collider;
	delete movement;
}

void DungeonObject::UpdateWorld()
{
	__super::UpdateWorld();
	collider->UpdateWorld();
}

void DungeonObject::Render()
{
	collider->Render();
}

void DungeonObject::SetPos(int x, int y)
{
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);

	if (!tileMap)
		return;

	Vector2 pos = tileMap->PointToPos({ x, y });
	movement->SetTargetPos(pos, 0.0f);
}

void DungeonObject::SetMovePlan(int dirX, int dirY, int dist)
{
	moveDirX = dirX;
	moveDirY = dirY;
	moveDist = dist;
}

bool DungeonObject::IsActing()
{
	if (movement->IsMoving() || moveDist > 0)
		return true;

	return false;
}

void DungeonObject::MovementUpdate()
{
	if (!movement->IsMoving()) {
		if (moveDist > 0)
			SetMove();
		else {
			//�̵��� �������� �˷��� �Ѵ�
			if(movement->IsMoved())
				Observer::Get()->ExecuteParamEvent("MoveEnd", (void*)this);
		}
	}
	movement->Update();
}

void DungeonObject::SetMove()
{
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);

	if (!tileMap)
		return;

	POINT unitPoint = tileMap->PosToPoint(GlobalPos());
	Vector2 destPos = {};
	if (tileMap->SetMove(unitPoint.x, unitPoint.y, moveDirX, moveDirY, destPos)) {
		movement->SetTargetPos(destPos);

		if(moveDist > 0)	//�� ���ư��� ��� moveDist == -1
			moveDist--;
	}
	else
		moveDist = 0;
}
