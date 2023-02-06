#include "Framework.h"
#include "ObjTileManager.h"
#include "../Unit/Unit.h"

ObjTileManager::ObjTileManager()
{
	Observer::Get()->AddParamEvent("UnitMoved", 
		bind(&ObjTileManager::ReserveActivate, this, placeholders::_1)
	);
}

ObjTileManager::~ObjTileManager()
{
}

void ObjTileManager::ReserveActivate(void* unit)
{
	
}

DungeonObjTile* ObjTileManager::Register(UINT x, UINT y, DungeonObjTile* obj)
{
	DungeonObjTile* post = nullptr;
	if (objTiles.find(make_pair(x, y)) != objTiles.end())
		post = objTiles[make_pair(x, y)];
	
	objTiles[make_pair(x, y)] = obj;
	//새로운 게 들어오면 먼저 것은 빼도록 한다 -> 보통 지워 없앨 용도


	return post;
}

void ObjTileManager::Clear()
{
	objTiles.clear();
}

DungeonObjTile* ObjTileManager::GetTile(UINT x, UINT y)
{
	if (objTiles.find(make_pair(x, y)) == objTiles.end())
		return nullptr;

	return objTiles[make_pair(x, y)];
}
