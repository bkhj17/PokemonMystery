#include "Framework.h"
#include "RobotMeleeAttack0118.h"

RobotMeleeAttack0118::RobotMeleeAttack0118(Transform* target)
	: RobotAttack0118("Melee.xml", target)
{
	power = 100.0f;

	attackCollider = new RectCollider(Vector2(70.0f, 120.0f));
	attackCollider->Pos().x = 50.0f;
	attackCollider->SetParent(target);
	attackCollider->SetActive(false);

	clip->SetEvent(bind(&RobotMeleeAttack0118::EnableCollider, this), 2);
	clip->SetEvent(bind(&RobotMeleeAttack0118::DisableCollider, this), 5);

	clip->SetEvent(bind(&RobotMeleeAttack0118::End, this));
}
