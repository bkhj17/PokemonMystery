#include "Framework.h"
#include "RobotJumpDown0118.h"

RobotJumpDown0118::RobotJumpDown0118(Transform* target)
	: Action("Textures/Robot/", "JumpDown.xml", false), target(target)
{
}

void RobotJumpDown0118::Update()
{
	velocity -= GRAVITY * DELTA;

	target->Pos().y += velocity * DELTA;

	if (KEY_PRESS(VK_RIGHT)) {
		target->Pos().x += MOVE_SPEED * DELTA;
		target->Rot().y = 0.f;
	}
	if (KEY_PRESS(VK_LEFT)) {
		target->Pos().x -= MOVE_SPEED * DELTA;
		target->Rot().y = XM_PI;
	}

	if (target->Pos().y < FLOOR_HEIGHT) {
		target->Pos().y = FLOOR_HEIGHT;
		velocity = 0.0f;

		if (event != nullptr)
			event();
	}
	Action::Update();
}

void RobotJumpDown0118::Start()
{
	Action::Start();
	velocity = 0.0f;
}
