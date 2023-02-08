#include "Framework.h"
#include "UnitMovement.h"

void UnitMovement::Update()
{
	if (!owner)
		return;

	if (time < 0.0f) {
		isMoved = false;
		return;
	}

	time -= speed * DELTA;

	owner->Pos() = Lerp(startPos, targetPos, (timeRate - time) * invTimeRate);
	isMoved = true;

	if (Distance(targetPos, owner->Pos()) < 1.0f)
		Teleport();
}

void UnitMovement::SetTargetPos(Vector2 pos, float time)
{
	startPos = owner->Pos();
	targetPos = pos;
	if (time <= 0.0f) {
		Teleport();
		return;
	}
	this->time = timeRate = time;
	invTimeRate = 1.0f / timeRate;
}

void UnitMovement::Teleport()
{
	owner->Pos() = targetPos;
	time = -1.0f;
}
