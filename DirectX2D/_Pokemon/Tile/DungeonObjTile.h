#pragma once

class DungeonObjTile : public Tile
{
private:
	friend class ObjTileManager;
public:
	DungeonObjTile(string type, Tile::Data data, Vector2 size);
	~DungeonObjTile() = default;

	void Update() {};
	void Render();

	string GetType() { return type; }
protected:
	string type = "";
};

