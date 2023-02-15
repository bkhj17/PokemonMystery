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
		TURN_END,
		FLOOR_MOVE,
		PLAYER_DEAD
	};

public:
	enum ShiftFloorState : int {
		POST = -1,
		NONE = 0,
		NEXT = 1,
		EXIT = 2
	};

	DungeonScene();
	~DungeonScene();


	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void CallTileMap(OUT void** tileMap) {
		if (this->tileMap == nullptr)
			return;

		*tileMap = (void*)this->tileMap;
	}

	void UnitMoveEnd(void* unitptr);

	void SetFloorMove(int floor) { this->floorMove = floor; }
	void ShiftNextFloor();
	void InitFloor(string name, int floor);

	void PlayerDie(void *unitPtr);
private:
	DungeonTileMap* tileMap;

	ActState actState = ENTER_DUNGEON;

	vector<Unit*> actionUnits;
	vector<Unit*> movingUnits;

	int floorMove = 0;

	//테스트
	Quad* gameOver;
};