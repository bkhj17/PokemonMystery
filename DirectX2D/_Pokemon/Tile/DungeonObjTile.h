#pragma once

class DungeonObjTile : public Tile
{
public:
	DungeonObjTile();
	~DungeonObjTile();


	string GetType() { return Type; }
protected:
	string Type = "";

};

