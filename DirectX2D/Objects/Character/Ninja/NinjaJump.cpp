#include "Framework.h"
#include "NinjaJump.h"

NinjaJump::NinjaJump(Transform* target)
	: Action("Textures/Ninja/", "Ninja_Jump.xml", false), target(target)
{
}

void NinjaJump::Update()
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

	if (target->Pos().y < FLOOR_HEIGHT) {
		target->Pos().y = FLOOR_HEIGHT;
		velocity = 0.0f;

		if(event != nullptr)
			event();
	}
	Action::Update();
}

void NinjaJump::Start()
{
	Action::Start();
	velocity = JUMP_POWER;
}