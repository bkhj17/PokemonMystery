#include "Framework.h"
#include "RobotShoot0118.h"

RobotShoot0118::RobotShoot0118(Transform* target)
	: Action("Textures/Robot/", "Shoot.xml", true), target(target)
{
	SetFrameEvents();
}

void RobotShoot0118::End()
{
	event();
}

void RobotShoot0118::Fire()
{
	if (fireFunc != nullptr) {
		fireFunc();
		return;
	}

	Vector2 firePos = target->Pos() + target->Right() * 20.0f;
	BulletManager::Get()->Fire(firePos, target->Right());
}

void RobotShoot0118::SetFrameEvents()
{
	clip->SetEvent(bind(&RobotShoot0118::Fire, this), 1);
	clip->SetEvent(bind(&RobotShoot0118::End, this));
}
