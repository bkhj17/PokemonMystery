#include "Framework.h"

DungeonObject::DungeonObject(Vector2 size)
{
	this->size = size;

	movement = new UnitMovement;
	movement->SetOwner(this);
}

DungeonObject::~DungeonObject()
{
	delete movement;
}

void DungeonObject::UpdateWorld()
{
	__super::UpdateWorld();
}

void DungeonObject::SetPoint(int x, int y)
{
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);

	if (!tileMap)
		return;

	Vector2 pos = tileMap->PointToPos({ x, y });
	movement->SetTargetPos(pos, 0.0f);
	UpdateWorld();
}

POINT DungeonObject::GetPoint()
{
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (tileMap == nullptr)
		return { -1, -1 };

	Vector2 pos = Pos();
	Vector2 movePos = movement->GetTargetPos();

	POINT point = tileMap->PosToPoint(IsMoving() ? movement->GetTargetPos() : Pos());
	return tileMap->PosToPoint(IsMoving() ? movement->GetTargetPos() : Pos());
}

void DungeonObject::SetMovePlan(int dirX, int dirY, int dist)
{
	moveDirX = dirX;
	moveDirY = dirY;
	moveDist = dist;
	MovementUpdate();
}

bool DungeonObject::IsActing()
{
	if (IsMoving())
		return true;

	return false;
}

bool DungeonObject::IsMoving()
{
	return movement->IsMoving() || moveDist > 0;
}

void DungeonObject::MovementUpdate()
{
	movement->Update();
	if (movement->IsMoving())
		return;

	//지점에 도착했지만 아직 더 갈 수 있다
	if (moveDist > 0) {
		if (IsCollide())	//뭔가에 부딪혔다
			CollideFunc();
		else	//아니면 속행
			SetMove();

		return;
	}
	
	//이동한 직후인 거라면 이동이 끝났음을 알려야 한다
	if(movement->IsMoved())
		Observer::Get()->ExecuteParamEvent("MoveEnd", (void*)this);
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

		if(moveDist > 0)	//쭉 날아가는 경우 moveDist == -1
			moveDist--;
	}
	else
		moveDist = 0;
}
