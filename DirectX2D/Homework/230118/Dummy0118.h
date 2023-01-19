#pragma once
class Dummy0118 : public GameObject, public Singleton<Dummy0118>
{
protected:
	enum ActionType {
		IDLE, HIT, DEAD
	};

	friend class Singleton;
	Dummy0118();
	virtual ~Dummy0118();

public:
	void Update();
	void Render();

	RectCollider* GetCollider() { return collider; }
	void Hit();
protected:

	void SetAction(ActionType type);
protected:
	map<ActionType, Action*> actions;
	ActionType curAction = IDLE;

	bool down = false;
	float downTime = 0.0f;
	float downRate = 2.0f;


	RectCollider* collider;
	float velocity = 0.0f;
};

