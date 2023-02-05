#pragma once

class Unit;
class Controller;
class DungeonTileMap;
class DungeonScene : public Scene
{
private:
	enum ActState {
		ENTER_DUNGEON,
		WAIT_COMMAND,
		ACTING,
		FLOOR_MOVE,
		PLAYER_DEAD
	};
public:
	DungeonScene();
	~DungeonScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void CallTileMap(OUT void** tileMap) {
		*tileMap = (void*)this->tileMap;
	}

private:
	DungeonTileMap* tileMap;
	Unit* testUnit;

	ActState actState = ENTER_DUNGEON;
};

