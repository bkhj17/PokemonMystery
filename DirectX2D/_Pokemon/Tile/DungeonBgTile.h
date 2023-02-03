#pragma once

class DungeonBgTile : public Tile
{
public:
	DungeonBgTile(Tile::Data data);
	DungeonBgTile(Tile::Data data, Vector2 size);
	~DungeonBgTile() = default;

	void Render() override;

	void UpdateGrid(int flag);
	bool IsRoom();

	Vector2 GetCutSize() { return cutSize; }
	int GetGridFlag() { return gridFrag; }
private:
	int gridFrag = 0;

	Vector2 cutSize;
};

