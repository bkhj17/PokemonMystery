#pragma once
#include "AnimTransform0117.h"

class Block0116 : public AnimTransform0117
{
private:
	const float FALLDOWN_SPEED = 20.0f;
	const int MAX_HIT_POINT = 2;

	enum ActionType {
		IDLE, HIT, DEAD
	};

public:
	Block0116(Vector2 Size);
	virtual ~Block0116();

	virtual void Update();
	void Render();

	void FallDown();

	RectCollider* GetCollider() { return collider; }

	void Spawn();

	void SetBreakFunc(function<void()> func) { breakFunc = func; }

	bool IsDead() { return curAction == DEAD; }

	void Hit();
	void Damaged();


private:
	void Break();
	void ChangeAction(ActionType action);


	virtual void LoadClips() override;

private:
	int hitPoint = MAX_HIT_POINT;

	ActionType curAction;

	float speed = FALLDOWN_SPEED;

	RectCollider* collider;
	function<void()> breakFunc;

	Vector2 size;

};

