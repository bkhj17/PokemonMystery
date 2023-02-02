#pragma once

class DungeonBgTile : public Tile
{
public:
	DungeonBgTile(Tile::Data data, Vector2 size);
	~DungeonBgTile() = default;

	void UpdateGrid(int flag);

	bool IsRoom();

private:
	int gridFrag = 0;

	Vector2 cutSize;
};

