#pragma once

class Pacman0130 : public GameObject
{
private:
	enum ActionState {
		IDLE, SUPER, DEAD
	};

public:
	Pacman0130();
	~Pacman0130();

	void Update();
	void Render();

	RectCollider* GetCollider() { return collider; }

	void UpdateWorld() override;

	void SetMoveLength(float moveLength) { this->moveLength = moveLength; }

	void SetPos(Vector2 pos);

	void GainSuper();
	void Dead();

	bool IsSuper() { return superTime > 0.0f; }
private:
	void CreateActions();

	void Control();
	void CheckInput();

	void SetAction(ActionState state);

private:
	RectCollider* collider;

	ActionState curAction = IDLE;
	map<UINT, Action*> actions;

	float spawnRate = 0.5f;
	float spawnTime = 0.0f;

	float superRate = 5.0f;
	float superTime = 0.0f;

	UINT lastKey = 0;
	float moveLength = 50.0f;

	float moveRate = 0.5f;
	float moveTime = 0.0f;

	Vector2 curPoint;
	Vector2 nextPoint;

	ColorBuffer* colorBuffer;
};