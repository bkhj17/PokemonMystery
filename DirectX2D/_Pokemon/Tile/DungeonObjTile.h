#pragma once

class DungeonObjTile : public Tile
{
public:
	DungeonObjTile(Tile::Data data, Vector2 size);
	~DungeonObjTile() = default;

	void Update() {};
	void Render();

	string GetType() { return Type; }
protected:
	string Type = "";
};

