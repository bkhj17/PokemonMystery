#include "Framework.h"
#include "UnitMovement.h"

void UnitMovement::Update()
{
	if (!owner)
		return;

	if (time < 0.0f)
		return;

	time -= DELTA;

	owner->Pos().x = Lerp(startPos.x, targetPos.x, (timeRate - time) * invTimeRate);
	owner->Pos().y = Lerp(startPos.y, targetPos.y, (timeRate - time) * invTimeRate);

	if (Distance(targetPos, owner->Pos()) < 5.0f) {
		Teleport();
	}
}

void UnitMovement::SetTargetPos(Vector2 pos, float time)
{
	if (time <= 0.0f) {
		Teleport();
	}

	startPos = owner->Pos();
	targetPos = pos;
	this->time = timeRate = time;
	invTimeRate = 1.0f / timeRate;
}

void UnitMovement::Teleport()
{
	owner->Pos() = targetPos;
	time = -1.0f;
}
