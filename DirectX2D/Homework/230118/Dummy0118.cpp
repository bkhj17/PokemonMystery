#include "Framework.h"
#include "Dummy0118.h"

Dummy0118::Dummy0118()
{
	string path = "Textures/Ninja/";
	actions[IDLE] = new Action(path, "Ninja_Idle.xml", true, 1.0f);
	actions[DEAD] = new Action(path, "Ninja_Dead.xml", false, 1.0f);

	collider = new RectCollider({80.0f, 150.0f});
	collider->SetParent(this);
}

Dummy0118::~Dummy0118()
{
	for (auto& action : actions)
		delete action.second;

	delete collider;
}

void Dummy0118::Update()
{
	if (!actions[curAction]->IsPlaying())
		SetAction(IDLE);

	UpdateWorld();
	collider->UpdateWorld();
}

void Dummy0118::Render()
{
	SetRender();
	actions[curAction]->Render();

	collider->Render();
}

void Dummy0118::SetAction(ActionType type)
{
	if (curAction == type)
		return;

	curAction = type;
	actions[type]->Start();
}
