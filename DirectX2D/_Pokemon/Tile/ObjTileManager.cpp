#include "Framework.h"
#include "ObjTileManager.h"
#include "../Unit/Unit.h"
#include "DungeonObjTile.h"

ObjTileManager::ObjTileManager()
{
	textures["Resfresh"] = L"Textures/pokemon/ObjectTile/Refresh.png";

	Observer::Get()->AddParamEvent("MoveEnd", 
		bind(&ObjTileManager::Activate, this, placeholders::_1)
	);
}

ObjTileManager::~ObjTileManager()
{
	//타일은 타일맵에서 지우기 때문에 삭제는 하지 않는다
}

void ObjTileManager::Activate(void* unitptr)
{
	auto unit = (Unit*)unitptr;
	if (unit == nullptr)
		return;

	pair<int, int> point = unit->GetPoint();
	if (objTiles.find(point) == objTiles.end())
		return;

	objTiles[unit->GetPoint()]->SetObject(unitptr);
	objTiles[unit->GetPoint()]->ExecuteEvent();
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
