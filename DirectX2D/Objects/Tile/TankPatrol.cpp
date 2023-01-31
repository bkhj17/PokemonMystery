#include "Framework.h"
#include "TankPatrol.h"

TankPatrol::TankPatrol(Transform* tank)
	: TankState(tank), checkTarget(&TankPatrol::CheckTarget, this)
{
	rangeCollider = new CircleCollider(RANGE);
	rangeCollider->SetParent(tank);

	Observer::Get()->AddParamEvent("TankPatrolSetTarget",
		bind(&TankPatrol::SetTarget, this, placeholders::_1));
}

TankPatrol::~TankPatrol()
{
	killOrder = true;
	checkTarget.join();
	delete rangeCollider;
}

void TankPatrol::Update()
{
	if (isPatrol) {
		Patrol();
	}
	else {
		stopTime += DELTA;
		if (stopTime > STOP_TIME) {
			Start();
		}
	}

	rangeCollider->UpdateWorld();
}

void TankPatrol::Render()
{
	rangeCollider->Render();
}

void TankPatrol::Start()
{
	isPatrol = true;
	stopTime = 0.0f;

	int start = aStar->FindCloseNode(tank->Pos());
	int end = aStar->FindRandomPos(tank->Pos(), RANGE);
	aStar->GetPath(start, end, path);
}

void TankPatrol::SetTarget(void* target)
{
	Isaac* isaac = (Isaac*)target;

	this->target = isaac;
}

void TankPatrol::Patrol()
{
	if (path.empty()) {
		isPatrol = false;
		return;
	}

	Vector2 direction = path.back() - tank->Pos();
	tank->Rot().z = direction.Angle();

	tank->Pos() += tank->Right() * speed * DELTA;

	if (direction.Length() < 5.0f)
		path.pop_back();
}

void TankPatrol::CheckTarget()
{
	float time = 0.0f;
	while (!killOrder) {
		time += DELTA;
		if (time < 0.1f)
			continue;

		time -= 0.1f;

		if (target == nullptr)
			continue;
		checkMutex.lock();

		float distance = Distance(tank->Pos(), target->Pos());
		if (distance < RANGE)
			Observer::Get()->ExecuteIntParamEvent("TankState", Tank::TRACE);
		checkMutex.unlock();
	}

}