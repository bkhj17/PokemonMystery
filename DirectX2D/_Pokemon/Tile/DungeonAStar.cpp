#include "Framework.h"
#include "DungeonAStar.h"
#include "DungeonTileMap.h"

DungeonAStar::DungeonAStar(DungeonTileMap* dungeonMap)
	: AStar(dungeonMap)
{
}

DungeonAStar::~DungeonAStar()
{
}
