#pragma once
class DungeonObjTile;
class ObjTileManager : public Singleton<ObjTileManager>
{
private:
	friend class Singleton;
	ObjTileManager();
	~ObjTileManager();


public:
	DungeonObjTile* Register(UINT x, UINT y, DungeonObjTile* obj);
	void Clear();

	DungeonObjTile* GetTile(UINT x, UINT y);

	void ReserveActivate(void* unit);

private:
	map<pair<UINT, UINT>, DungeonObjTile*> objTiles;
};

