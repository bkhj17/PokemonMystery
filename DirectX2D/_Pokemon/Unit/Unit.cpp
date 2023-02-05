#include "Framework.h"
#include "Unit.h"
#include "UnitMovement.h"
#include "../Control/Controller.h"
#include "../Tile/DungeonTileMap.h"

Unit::Unit(Vector2 size)
{
	this->size = size;

	animObject = new AnimObject();
	animObject->SetParent(this);

	collider = new RectCollider(size);
	collider->SetParent(this);

	movement = new UnitMovement();
	movement->SetOwner(this);
}

Unit::Unit(Controller* controller, Vector2 size)
	: controller(controller)
{
	this->size = size;

	animObject = new AnimObject();
	animObject->SetParent(this);

	collider = new RectCollider(size);
	collider->SetParent(this);

	movement = new UnitMovement();
	movement->SetOwner(this);

	controller->SetUnit(this);
}

Unit::~Unit()
{
	delete animObject;
	delete collider;
	delete movement;
}

void Unit::Update()
{
	movement->Update();
	
	//animObject->Update();
	UpdateWorld();
}

void Unit::Render()
{
	__super::SetRender();
	//animObject->Render();
	collider->Render();
}

void Unit::SetClipData()
{
}

void Unit::UpdateWorld()
{
	__super::UpdateWorld();
	animObject->UpdateWorld();
	collider->UpdateWorld();
}

void Unit::SetController(Controller* controller)
{
	this->controller = controller;
	controller->SetUnit(this);
}

void Unit::SetDir(int x, int y)
{
	animObject->SetDir(x, y);
}

void Unit::SetPos(int x, int y)
{
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);

	if (!tileMap)
		return;

	Vector2 pos = tileMap->PointToPos({ x, y });
	movement->SetTargetPos(pos, 0.0f);
}

void Unit::SetMove(int dirX, int dirY)
{
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);

	if (!tileMap)
		return;

	POINT unitPoint = tileMap->PosToPoint(GlobalPos());
	Vector2 destPos = {};
	if (tileMap->SetMove(unitPoint.x, unitPoint.y, dirX, dirY, destPos))
		movement->SetTargetPos(destPos);
}

bool Unit::IsActing()
{
	if (movement->IsMoving())
		return true;

	return false;
}
