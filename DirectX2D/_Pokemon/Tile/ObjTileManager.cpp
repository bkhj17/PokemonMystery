#include "Framework.h"
#include "ObjTileManager.h"
#include "../Unit/Unit.h"
#include "DungeonObjTile.h"

ObjTileManager::ObjTileManager()
{
	textures["Refresh"] = L"Textures/pokemon/ObjectTile/Refresh.png";

	Observer::Get()->AddParamEvent("MoveEnd", 
		bind(&ObjTileManager::Activate, this, placeholders::_1)
	);
}

void ObjTileManager::Activate(void* unitptr)
{
	auto unit = (Unit*)unitptr;
	if (unit == nullptr)
		return;

	pair<int, int> point = {unit->GetPoint().x, unit->GetPoint().y};
	if (objTiles.find(point) == objTiles.end())
		return;

	objTiles[point]->SetObject(unitptr);
	objTiles[point]->ExecuteEvent();
}

wstring ObjTileManager::GetTextureFile(string key)
{
	if (textures.find(key) == textures.end())
		return L"";

	return textures[key];
}

DungeonObjTile* ObjTileManager::Register(int x, int y, DungeonObjTile* obj)
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

DungeonObjTile* ObjTileManager::GetTile(int x, int y)
{
	if (objTiles.find(make_pair(x, y)) == objTiles.end())
		return nullptr;

	return objTiles[make_pair(x, y)];
}
