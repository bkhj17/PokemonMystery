#pragma once

class Tank : public Quad
{
public:
	enum State {
		PATROL, TRACE, ATTACK,
		HIT, DEAD
	};

public:
	Tank();
	Tank(AStar* astar);
	~Tank();

	virtual void Update();
	virtual void UpdateWorld() override;
	void Render();

	void PostRender();

	RectCollider* GetCollider() { return collider; }
	void SetTarget(Transform* target) { this->target = target; }

	//
	void SetPath(vector<Vector2> path) { this->path = path; }
	vector<Vector2>& GetPath() { return path; }

	virtual void Fire();
	string GetShooterTag() { return shooterTag; }

	void Destroy();
	void SetDestroyEvent(Event event) { destroyEvent = event; }

protected:
	Vector2 GetFirePoint();

	virtual void Control();
	virtual void HeadControl();

	void MovePath();

private:
	void CreateStates();
	void SetState(int state);

protected:
	string shooterTag = "Tank";

	float moveSpeed = 200.0f;
	float rotSpeed = 5.0f;

private:
	RectCollider* collider;
	Quad* head;

	map<State, TankState*> states;
	TankState* curState = nullptr;
	
	AStar* astar;
	Transform* target = nullptr;
	//
	vector<Vector2> path;
	Event destroyEvent;
};

