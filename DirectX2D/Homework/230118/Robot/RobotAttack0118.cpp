#include "Framework.h"
#include "RobotAttack0118.h"
#include "../Dummy0118.h"

RobotAttack0118::RobotAttack0118(string file, Transform* target)
	: Action("Textures/Robot/", file, false), target(target)
{
}

RobotAttack0118::~RobotAttack0118()
{
	delete attackCollider;
}

void RobotAttack0118::Update()
{
	__super::Update();
	attackCollider->UpdateWorld();

	if(attackCollider->Active() && attackCollider->IsCollision(Dummy0118::Get()->GetCollider()))
		Dummy0118::Get()->Hit();
}

void RobotAttack0118::Render()
{
	Action::Render();

	attackCollider->Render();
}

void RobotAttack0118::End()
{
	event();
}

void RobotAttack0118::EnableCollider()
{
	attackCollider->SetActive(true);
	attackCollider->UpdateWorld();
}

void RobotAttack0118::DisableCollider()
{
	attackCollider->SetActive(false);
	attackCollider->UpdateWorld();
}
