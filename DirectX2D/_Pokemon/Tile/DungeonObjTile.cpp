#include "Framework.h"
#include "DungeonObjTile.h"

DungeonObjTile::DungeonObjTile(Tile::Data data, Vector2 size)
	: Tile(data, size)
{
	event = []() {
		Observer::Get()->ExecuteEvent("ShiftNextFloor");
	};

	paramEvent = nullptr;
	object = nullptr;
}

void DungeonObjTile::Render()
{
	Quad::Render();
}
