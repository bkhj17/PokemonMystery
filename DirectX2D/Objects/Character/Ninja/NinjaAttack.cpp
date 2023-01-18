#include "Framework.h"
#include "NinjaAttack.h"

NinjaAttack::NinjaAttack(string file, Transform* target)
	: Action("Textures/Ninja/", file, false), target(target)
{
}

NinjaAttack::~NinjaAttack()
{
	delete attackCollider;
}

void NinjaAttack::Render()
{
	Action::Render();

	attackCollider->Render();
}

void NinjaAttack::End()
{
	event();
}

void NinjaAttack::EnableCollider()
{
	attackCollider->SetActive(true);
	attackCollider->UpdateWorld();
}

void NinjaAttack::DisableCollider()
{
	attackCollider->SetActive(false);
	attackCollider->UpdateWorld();
}
