#include "Framework.h"
#include "Pacman0130.h"

Pacman0130::Pacman0130()
{
	CreateActions();
	actions[curAction]->Start();
	collider = new RectCollider(actions[curAction]->GetCurFrameSize());
	collider->SetParent(this);

	Scale() = Scale() / actions[curAction]->GetCurFrameSize();
	__super::UpdateWorld();

	colorBuffer = new ColorBuffer;
	colorBuffer->Get() = { 1.0f, 1.0f, 1.0f, 1.0f };
}

Pacman0130::~Pacman0130()
{
	delete collider;
	for(auto& action : actions) 
		delete action.second;

	delete colorBuffer;
}

void Pacman0130::Update()
{
	if (spawnTime > 0.0f) {
		spawnTime -= DELTA;
		if (spawnTime > 0.0f)
			return;
		else
			SetAction(IDLE);
	}
	
	if (superTime > 0.0f) {
		superTime -= DELTA;
	}

	Control();
	actions[curAction]->Update();

}

void Pacman0130::Render()
{
	if (spawnTime > 0.0f)
		return;
	colorBuffer->SetPS(0);

	SetRender();
	actions[curAction]->Render();
}

void Pacman0130::UpdateWorld()
{
	__super::UpdateWorld();
	collider->UpdateWorld();
}

void Pacman0130::SetPos(Vector2 pos)
{
	curPoint = Pos() = nextPoint = pos;
}

void Pacman0130::GainSuper()
{
	superTime = superRate;
}

void Pacman0130::CreateActions()
{
	actions[IDLE] = new Action("Textures/Pacman/", "Idle.xml", true);
	actions[DEAD] = new Action("Textures/Pacman/", "Dead.xml", false);
	actions[DEAD]->SetEvent(bind(&Pacman0130::Dead, this));
}

void Pacman0130::Control()
{
	Vector2 pos = Pos();
	if ((nextPoint - Pos()).Length() < 0.5f) {
		Pos() = curPoint = nextPoint;
		CheckInput();
		moveTime = 0.0f;
	}
	else {
		moveTime += 500.0f * DELTA * DELTA;
		Pos() = Lerp(curPoint, nextPoint, moveTime / moveRate);
	}

	UpdateWorld();
}

void Pacman0130::CheckInput()
{
	if (KEY_PRESS(VK_RIGHT)) {
		Rot().z = 0.0f;
		UpdateWorld();
		nextPoint = curPoint + Right() * moveLength;
		return;
	}
	else if (KEY_PRESS(VK_LEFT)) {
		Rot().z = XM_PI;
		UpdateWorld();
		nextPoint = curPoint + Right() * moveLength;
		return;
	}
	else if (KEY_PRESS(VK_UP)) {
		Rot().z = XM_PIDIV2;
		UpdateWorld();
		nextPoint = curPoint + Right() * moveLength;
		return;
	}
	else if (KEY_PRESS(VK_DOWN)) {
		Rot().z = -XM_PIDIV2;
		UpdateWorld();
		nextPoint = curPoint + Right() * moveLength;
	}
}

void Pacman0130::SetAction(ActionState state)
{
	if (curAction == state)
		return;

	curAction = state;
	actions[curAction]->Start();
}

void Pacman0130::Dead()
{
	spawnTime = spawnRate;
}
