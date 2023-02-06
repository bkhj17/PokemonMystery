#include "Framework.h"
#include "DungeonObjTile.h"

DungeonObjTile::DungeonObjTile(Tile::Data data, Vector2 size)
	: Tile(data, size)
{
}

void DungeonObjTile::Render()
{
	Quad::Render();
}
