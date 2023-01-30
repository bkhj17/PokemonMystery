#pragma once
class TileMap0130 : public GameTileMap
{
private:
	enum CollideSet {
		WALL,
		POWER,
		CHIP
	};

	map<wstring, CollideSet> collideTable;
public:
	TileMap0130(string file);
	~TileMap0130() = default;


	virtual void PushObject(Collider* collider) override;
	void ItemCollision(Collider* collider);
	
	virtual void GetNodes(vector<Node*>& nodes);
	
	virtual void Render() override;



	virtual void UpdateWorld() override;

	Vector2 GetTileSize() { return tileSize * Scale(); }

	void SetPushEvent(Event event) { pushEvent = event; }
	void SetWinEvent(Event event) { winEvent = event; }
	void SetPowerEvent(Event event) { powerEvent = event; }
private:
	void SetUp();

	void CheckItem();
private:
	vector<Tile*> walls;
	vector<Tile*> powers;
	vector<Tile*> chips;

	Event pushEvent;
	Event winEvent;
	Event powerEvent;

	int numItem = 0;
};

