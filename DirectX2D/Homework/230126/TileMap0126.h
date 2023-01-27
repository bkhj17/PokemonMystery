#pragma once

class TileMap0126 : public GameTileMap
{
	enum CollideSet {
		SPAWN_POINT,
		BREAKABLE,
		UNBREAKABLE,
		PROTECT,
		PLAYER_SPAWN
	};

	map<wstring, CollideSet> collideTable;

public:
	TileMap0126(string file);
	~TileMap0126() = default;

	void BulletCollision();

	virtual void PushObject(Collider* collider) override;
	virtual void Render() override;

	void SetLoseEvent(Event event) { loseEvent = event; }

	Vector2 EnemySpawnPoint();
	Vector2 PlayerSpawnPoint() { return playerSpawn + GlobalPos(); }

	bool PointCollision(Vector2 pos);
	Vector2 GetTileSize() { return tileSize; }
private:
	void SetUp();

	void BreakProtect(Tile* protect);
private:
	vector<Tile*> enemySpawns;
	vector<Tile*> unbreakables;
	vector<Tile*> breakables;
	vector<Tile*> protects;

	int protectNum = 0;
	Event loseEvent;

	Vector2 playerSpawn = {};
};

