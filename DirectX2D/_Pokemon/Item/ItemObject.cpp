#include "Framework.h"
#include "ItemObject.h"
#include "../Unit/UnitMovement.h"

ItemObject::ItemObject(string key, Vector2 size)
	: DungeonObject(size)
{
}

void ItemObject::Update()
{
	movement->Update();
	UpdateWorld();
}
