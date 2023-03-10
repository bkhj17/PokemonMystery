#pragma once
class ObjectMovement
{
public:
	ObjectMovement() = default;
	~ObjectMovement() = default;

	void Update();

	void SetTargetPos(Vector2 pos, float time = 1.0f);
	void SetOwner(Transform* owner) { this->owner = owner; }

	bool IsMoving() { return time > 0.0f; }
	bool IsMoved() { return isMoved; }

	Vector2 GetTargetPos() { return targetPos; }
private:
	void Teleport();
private:
	Transform* owner = nullptr;

	Vector2 startPos = { 0, 0 };
	Vector2 targetPos = { 0, 0 };
	float timeRate = 1.0f;
	float invTimeRate = 1.0f;
	float time = -1.0f;

	float speed = 5.0f;

	//?̵? ?Ÿ? - 2ĭ ???ų? ?????? ?΋H?? ?????? ?????̴? ??ü?? ?ִ?
	bool isMoved = false;
};

