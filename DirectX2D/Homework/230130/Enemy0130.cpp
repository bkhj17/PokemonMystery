#include "Framework.h"
#include "Enemy0130.h"

Enemy0130::Enemy0130()
{
	CreateActions();
	actions[curAction]->Start();
	collider = new RectCollider(actions[curAction]->GetCurFrameSize());
	collider->SetParent(this);

	Scale() = Scale() / actions[curAction]->GetCurFrameSize();
	__super::UpdateWorld();

	colorBuffer = new ColorBuffer;
	colorBuffer->Get() = WHITE;
}

Enemy0130::~Enemy0130()
{
	delete collider;
	for (auto& action : actions)
		delete action.second;
	delete colorBuffer;
}

void Enemy0130::Update()
{
	actions[curAction]->Update();
	if (path.empty())
		Observer::Get()->ExecuteParamEvent("EnemyCallPath", this);

	MovePath();

	UpdateWorld();
}

void Enemy0130::UpdateWorld()
{
	__super::UpdateWorld();
	collider->UpdateWorld();
}

void Enemy0130::Render()
{
	if (!isActive)
		return;

	SetRender();
	colorBuffer->SetPS(0);
	actions[curAction]->Render();
}

void Enemy0130::PostRender()
{
}

void Enemy0130::Destroy()
{
	isActive = false;

	if (destroyEvent)
		destroyEvent();
}

Vector2 Enemy0130::GetTargetPos()
{
	Vector2 target = Pos();
	switch (curAction)
	{
	case Enemy0130::PATROL:
	{
		target.x += Random(-100.0f, 100.0f);
		target.y += Random(-100.0f, 100.0f);
	}
	break;
	case Enemy0130::CHASE:
	{
		//플레이어 쫓기
		Observer::Get()->ExecuteParamEvent("GetPlayerPos", &target);
	}
	break;
	case Enemy0130::SCARED:
		break;
	default:
		break;
	}
	return target;
}

void Enemy0130::CreateActions()
{
	actions[PATROL] = new Action("Textures/Pacman/", "EnemyBody.xml", true);
	actions[CHASE] = new Action("Textures/Pacman/", "EnemyBody.xml", true);
	actions[SCARED] = new Action("Textures/Pacman/", "EnemyScared.xml", true);
}

void Enemy0130::SetAction(ActionState state)
{
	if (curAction == state)
		return;

	switch (state)
	{
	case Enemy0130::PATROL:
		colorBuffer->Get() = WHITE;
		break;
	case Enemy0130::CHASE:
		path.clear();
		colorBuffer->Get() = MAGENTA;
		break;
	case Enemy0130::SCARED:
		path.clear();
		break;
	default:
		break;
	}

	curAction = state;
	actions[curAction]->Start();
}

void Enemy0130::MovePath()
{
	if (path.empty())
		return;

	Vector2 direction = path.back() - Pos();
	Rot().z = direction.Angle();

	Pos() += Right() * moveSpeed * DELTA;

	if (direction.Length() < 5.0f)
		path.pop_back();

}