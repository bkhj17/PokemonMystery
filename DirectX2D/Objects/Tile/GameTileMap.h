#pragma once

class GameTileMap : public Transform
{
public:
	GameTileMap() {};
	GameTileMap(string file);
	virtual ~GameTileMap();

	virtual void UpdateWorld() override;
	virtual void Render();

	virtual void PushObject(Collider* collider);
	virtual void PushRect(RectCollider* collider);

	virtual void GetNodes(vector<Node*>& nodes);

	UINT GetWidth() { return width; }
	UINT GetHeight() { return height; }

	Vector2 LeftBottom();
	Vector2 RightTop();
protected:
	virtual void Load(string file);

protected:
	UINT width = 0, height = 0;

	Vector2 tileSize;

	vector<Quad*> bgTiles;
	vector<Tile*> objTiles;
};

