#include "Framework.h"
#include "NinjaRun.h"

NinjaRun::NinjaRun(Transform* target)
	: Action("Textures/Ninja/", "Ninja_Run.xml", true), target(target)
{
}

NinjaRun::NinjaRun(Transform* target, string path, string file, float speed)
	: Action(path, file, true, speed), target(target)
{
}

void NinjaRun::Update()
{
	Control();

	target->Pos().x += velocity * speed * DELTA;

	Action::Update();
}

void NinjaRun::Control()
{
	if (KEY_PRESS(VK_RIGHT)) {
		velocity = 1.0f;
		target->Rotation().y = 0.f;
	}

	if (KEY_PRESS(VK_LEFT)) {
		velocity = -1.0f;
		target->Rotation().y = XM_PI;
	}

}
