#pragma once

class DungeonBgTile : public Quad
{
public:
	DungeonBgTile(Tile::Data data);
	DungeonBgTile(Tile::Data data, Vector2 size);
	~DungeonBgTile() = default;

	void UpdateGrid(int flag);
	bool IsRoom();

	Vector2 GetCutSize() { return cutSize; }
	int GetGridFlag() { return gridFrag; }

	Tile::Data& GetData() { return data; }
	void SetData(Tile::Data data);
private:
	int gridFrag = 0;

	Vector2 cutSize;

	Tile::Data data;
};

