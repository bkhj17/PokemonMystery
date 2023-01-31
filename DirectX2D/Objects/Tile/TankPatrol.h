#pragma once
class TankPatrol : public TankState
{
private:
	const float RANGE = 200.0f;
	const float STOP_TIME = 1.0f;

public:
	TankPatrol(Transform* tank);
	~TankPatrol();

	void Update();
	void Render();

	void Start();

	void SetAStar(AStar* aStar) { this->aStar = aStar; }
	void SetTarget(void* target);

private:
	void Patrol();
	void CheckTarget();
private:
	bool killOrder = false;

	CircleCollider* rangeCollider;
	AStar* aStar = nullptr;

	bool isPatrol = false;
	float stopTime = 0.0f;

	vector<Vector2> path;

	Transform* target = nullptr;

	thread checkTarget;
	mutex checkMutex;
};

