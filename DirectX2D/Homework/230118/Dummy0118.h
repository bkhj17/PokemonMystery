#pragma once
class Dummy0118 : public GameObject, public Singleton<Dummy0118>
{
private:
	enum ActionType {
		IDLE, HIT, DEAD
	};

	friend class Singleton;
	Dummy0118();
	~Dummy0118();

public:
	void Update();
	void Render();

	RectCollider* GetCollider() { return collider; }
	void Hit();
private:

	void SetAction(ActionType type);
private:
	map<ActionType, Action*> actions;
	ActionType curAction = IDLE;

	RectCollider* collider;
	float velocity = 0.0f;
};

