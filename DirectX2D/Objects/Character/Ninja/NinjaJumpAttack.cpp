#include "Framework.h"
#include "NinjaJumpAttack.h"

NinjaJumpAttack::NinjaJumpAttack(Transform* target)
	: NinjaAttack("Ninja_JumpAttack.xml", target)
{
	power = 100.0f;

	attackCollider = new RectCollider(Vector2(70.0f, 120.0f));
	attackCollider->Pos().x = 50.0f;
	attackCollider->SetParent(target);
	attackCollider->SetActive(false);

	clip->SetEvent(bind(&NinjaJumpAttack::EnableCollider, this), 4);
	clip->SetEvent(bind(&NinjaJumpAttack::DisableCollider, this), 7);
	
}

void NinjaJumpAttack::Update()
{
	NinjaAttack::Update();

	if (KEY_PRESS(VK_RIGHT)) {
		target->Pos().x += MOVE_SPEED * DELTA;
		target->Rotation().y = 0.f;
	}
	if (KEY_PRESS(VK_LEFT)) {
		target->Pos().x -= MOVE_SPEED * DELTA;
		target->Rotation().y = XM_PI;
	}

	if (isFall) {
		velocity -= GRAVITY * DELTA;
		target->Pos().y += velocity * DELTA;

		if (target->Pos().y < FLOOR_HEIGHT) {
			target->Pos().y = FLOOR_HEIGHT;

			velocity = 0.0f;

			if (event != nullptr)
				event();
		}
	}
}

void NinjaJumpAttack::Start()
{
	NinjaAttack::Start();
}

void NinjaJumpAttack::DisableCollider()
{
	NinjaAttack::DisableCollider();

	velocity = 0.0f;
	isFall = true;
}
