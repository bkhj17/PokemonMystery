#pragma once
class EditTileMap : public Transform
{
public:
	EditTileMap(UINT width, UINT height);
	~EditTileMap();

	void Update();
	void Render();

	void ClickTile(Quad* selectSample, Tile::Type type);
	void DeleteObjTile();

	void Save(string file);
	void Load(string file);
private:
	void CreateTile();

	void SetBGTile(wstring file, float angle);
	void SetOBJTile(wstring file, float angle);
private:
	UINT width, height;

	vector<Tile*> bgTiles;
	deque<Tile*> objTiles;

	Vector2 tileSize;
};

