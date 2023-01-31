#pragma once
#include "TankState.h"
class TankTrace :
    public TankState
{
private:
	const float PATROL_RANGE = 200.0f;
	const float ATTACK_RANGE = 100.0f;

	const float UPDATE_PATH_TIME = 0.5f;
public:
	TankTrace(Transform* tank);
	~TankTrace();

	void Update();
	void Render();

	void Start();

	void SetAStar(AStar* aStar) { this->aStar = aStar; }
	void SetTarget(void * target);

private:
	void Move();
	void Trace();
	void CheckTarget();

private:
	bool killOrder = false;
	
	CircleCollider* rangeCollider;
	AStar* aStar = nullptr;
	
	vector<Vector2> path;

	Transform* target = nullptr;
	float updateTime = 0.0f;

	thread checkTarget;
	mutex checkMutex;
};

