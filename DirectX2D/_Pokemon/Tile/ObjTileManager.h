#pragma once
class DungeonObjTile;
class ObjTileManager : public Singleton<ObjTileManager>
{
private:
	friend class Singleton;
	ObjTileManager();
	~ObjTileManager();

public:
	DungeonObjTile* Register(int x, int y, DungeonObjTile* obj);
	void Clear();

	DungeonObjTile* GetTile(int x, int y);

	void Activate(void* unitptr);

private:
	map<pair<int, int>, DungeonObjTile*> objTiles;	
};

