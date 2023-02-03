#pragma once

class UnitMovement;
class Unit : public GameObject
{
public:
	Unit(Vector2 size);
	~Unit();

	void Update();
	void Render();

	void SetClipData();
	void UpdateWorld();

	UnitMovement* GetMovement() { return movement; }
private:
	AnimObject* animObject;
	RectCollider* collider;

	UnitMovement* movement;
};

