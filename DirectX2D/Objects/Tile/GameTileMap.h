#pragma once

class GameTileMap : public Transform
{
public:
	GameTileMap(string file);
	virtual ~GameTileMap();

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
	UINT width, height;

	Vector2 tileSize;

	vector<Quad*> bgTiles;
	vector<Tile*> objTiles;
};

