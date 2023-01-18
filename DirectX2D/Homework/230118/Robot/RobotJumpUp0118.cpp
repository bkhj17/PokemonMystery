#include "Framework.h"
#include "RobotJumpUp0118.h"

RobotJumpUp0118::RobotJumpUp0118(Transform* target)
	: Action("Textures/Robot/", "JumpUp.xml", false), target(target)
{
}

void RobotJumpUp0118::Update()
{
	velocity -= GRAVITY * DELTA;
	target->Pos().y += velocity * DELTA;

	if (KEY_PRESS(VK_RIGHT)) {
		target->Pos().x += MOVE_SPEED * DELTA;
		target->Rotation().y = 0.f;
	}
	if (KEY_PRESS(VK_LEFT)) {
		target->Pos().x -= MOVE_SPEED * DELTA;
		target->Rotation().y = XM_PI;
	}

	if (velocity < 0.0f) {
		if (event != nullptr)
			event();
	}
	Action::Update();
}

void RobotJumpUp0118::Start()
{
	Action::Start();
	velocity = JUMP_POWER;
}
