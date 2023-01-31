#include "Framework.h"
#include "TankTrace.h"

TankTrace::TankTrace(Transform* tank)
	:TankState(tank), checkTarget(&TankTrace::CheckTarget, this)
{
	rangeCollider = new CircleCollider(ATTACK_RANGE);
	rangeCollider->SetParent(tank);
	rangeCollider->GetColor() = { 1, 0, 0, 1 };

	Observer::Get()->AddParamEvent("TankTraceSetTarget",
		bind(&TankTrace::SetTarget, this, placeholders::_1));
}

TankTrace::~TankTrace()
{
	killOrder = true;
	checkTarget.join();
	delete rangeCollider;
}

void TankTrace::Update()
{
	updateTime += DELTA;
	if (updateTime > UPDATE_PATH_TIME) {
		updateTime -= UPDATE_PATH_TIME;
		Trace();
	}

	Move();
	//CheckTarget();

	rangeCollider->UpdateWorld();
}

void TankTrace::Render()
{
	rangeCollider->Render();
}

void TankTrace::Start()
{
	updateTime = 0.0f;
}

void TankTrace::SetTarget(void* target)
{
	Isaac* isaac = (Isaac*)target;

	this->target = isaac;
}

void TankTrace::Move()
{
	if (path.empty())
		return;

	Vector2 direction = path.back() - tank->Pos();
	tank->Rot().z = direction.Angle();

	tank->Pos() += tank->Right() * speed * DELTA;

	if (direction.Length() < 5.0f)
		path.pop_back();
}

void TankTrace::Trace()
{
	int start = aStar->FindCloseNode(tank->Pos());
	int end = aStar->FindCloseNode(target->Pos());

	aStar->GetPath(start, end, path);
}

void TankTrace::CheckTarget()
{
	float time = 0.0f;

	while (!killOrder) {
		time += DELTA;
		if (time < 0.1f)
			continue;

		time -= 0.1f;

		if (tank == nullptr || target == nullptr)
			continue;

		checkMutex.lock();
		float distance = Distance(tank->Pos(), target->Pos());
		if (distance > PATROL_RANGE)
			Observer::Get()->ExecuteIntParamEvent("TankState", Tank::PATROL);
		checkMutex.unlock();
	}
}
