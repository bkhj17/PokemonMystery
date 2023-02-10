#pragma once
class DungeonObjTile;
class ObjTileManager : public Singleton<ObjTileManager>
{
private:
	friend class Singleton;
	ObjTileManager();
	~ObjTileManager() = default;
	//타일은 타일맵에서 지우기 때문에 삭제는 하지 않는다

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

