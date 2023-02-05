#pragma once

class UnitMovement;
class Controller;
class Unit : public GameObject
{
public:
	Unit(Vector2 size);
	Unit(Controller* controller, Vector2 size);
	~Unit();

	void Update();
	void Render();

	void SetClipData();
	void UpdateWorld();

	Controller* GetController() { return controller; }
	void SetController(Controller* controller);

	UnitMovement* GetMovement() { return movement; }

	void SetDir(int x, int y);
	void SetPos(int x, int y);
	void SetMove(int dirX, int dirY);

	bool IsActing();
private:
	AnimObject* animObject;

	RectCollider* collider;

	UnitMovement* movement;

	Controller* controller = nullptr;
};

