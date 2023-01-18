#include "Framework.h"
#include "NinjaMeleeAttack.h"

NinjaMeleeAttack::NinjaMeleeAttack(Transform* target)
	: NinjaAttack("Ninja_Attack.xml", target)
{
	power = 100.0f;

	attackCollider = new RectCollider(Vector2(70.0f, 120.0f));
	attackCollider->Pos().x = 50.0f;
	attackCollider->SetParent(target);
	attackCollider->SetActive(false);

	clip->SetEvent(bind(&NinjaMeleeAttack::EnableCollider, this), 4);
	clip->SetEvent(bind(&NinjaMeleeAttack::DisableCollider, this), 7);

	clip->SetEvent(bind(&NinjaMeleeAttack::End, this));
}
