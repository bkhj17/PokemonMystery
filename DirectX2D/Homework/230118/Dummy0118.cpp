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
	actions[curAction]->Update();

	velocity -= 980.0f * DELTA;
	localPosition.y += velocity * DELTA;
	if (localPosition.y < 200.0f) {
		localPosition.y = 200.0f;
		velocity = 0.0f;
		if(curAction == DEAD)
			down = true;
	}

	if (down) {
		downTime += DELTA;
		if (downTime > downRate) {
			downTime = 0.0f;
			down = false;
			SetAction(IDLE);
		}
	}

	UpdateWorld();
	collider->UpdateWorld();
}

void Dummy0118::Render()
{
	SetRender();
	actions[curAction]->Render();

	collider->Render();
}

void Dummy0118::Hit()
{
	if (down)
		return;

	velocity = 500.0f;

	SetAction(DEAD);
}

void Dummy0118::SetAction(ActionType type)
{
	if (curAction == type && type != DEAD)
		return;

	curAction = type;
	actions[type]->Start();
}
