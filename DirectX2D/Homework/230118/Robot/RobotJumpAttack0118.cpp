#include "Framework.h"
#include "RobotJumpAttack0118.h"

RobotJumpAttack0118::RobotJumpAttack0118(Transform* target)
	: RobotAttack0118("JumpMelee.xml", target)
{
	power = 100.0f;

	attackCollider = new RectCollider(Vector2(70.0f, 120.0f));
	attackCollider->Pos().x = 50.0f;
	attackCollider->SetParent(target);
	attackCollider->SetActive(false);

	clip->SetEvent(bind(&RobotJumpAttack0118::EnableCollider, this), 4);
	clip->SetEvent(bind(&RobotJumpAttack0118::DisableCollider, this), 7);
	clip->SetEvent(bind(&RobotJumpAttack0118::End, this));
}
