#include "Framework.h"
#include "MiniMap.h"
#include "../Tile/DungeonTileMap.h"
#include "../Tile/DungeonBgTile.h"

MiniMap::MiniMap()
{
	quad = new Quad(Vector2(20.0f, 20.0f));
}

MiniMap::~MiniMap()
{
	delete quad;
}

void MiniMap::Init()
{
	//가지고 있던 정보를 전부 없앤다
	Clear();
}

void MiniMap::UpdateMap(void* pointPtr)
{
	POINT* point = (POINT*)pointPtr;
	if (!point)
		return;

	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);

	if (!tileMap)
		return;

	vector<POINT> detectPoints = tileMap->DetectableTiles(*point);
	for (const auto& detect : detectPoints)
		info[detect] = tileMap->GetBgTile(detect)->GetGridFlag();
}

void MiniMap::Render()
{
}

void MiniMap::Clear()
{
	info.clear();
	objTilePos.clear();
	itemPos.clear();
	unitPos.clear();
}
