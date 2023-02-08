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

	wstring GetTextureFile(string key);

private:
	unordered_map<string, wstring> textures;
	map<pair<int, int>, DungeonObjTile*> objTiles;
};

