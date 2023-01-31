#include "Framework.h"

Tank::Tank()
	: Quad(L"Textures/Shooting/Tank_Body.png"), astar(nullptr)
{
	collider = new RectCollider(texture->GetSize());
	collider->SetTag("TankCollider");
	collider->Load();
	collider->SetParent(this);

	head = new Quad(L"Textures/Shooting/Tank_Head.png");
	head->SetParent(this);

	Observer::Get()->AddIntParamEvent("TankState", bind(&Tank::SetState, this, placeholders::_1));
}

Tank::Tank(AStar* astar)
	: Quad(L"Textures/Shooting/Tank_Body.png"), astar(astar)
{
	CreateStates();
	curState = states[PATROL];

	collider = new RectCollider(texture->GetSize());
	collider->SetTag("TankCollider");
	collider->Load();
	collider->SetParent(this);

	head = new Quad(L"Textures/Shooting/Tank_Head.png");
	head->SetParent(this);

	Observer::Get()->AddIntParamEvent("TankState",
		bind(&Tank::SetState, this, placeholders::_1));
}

Tank::~Tank()
{
	delete collider;
	delete head;

	for (auto& state : states)
		delete state.second;
}

void Tank::Update()
{
	Action();
	//Control();
	//HeadControl();
	//MovePath();
	curState->Update();

	UpdateWorld();

	BulletManager::Get()->CheckCollision(GetCollider(), bind(&Tank::Destroy, this), shooterTag);
}

void Tank::UpdateWorld()
{
	__super::UpdateWorld();
	collider->UpdateWorld();
	head->UpdateWorld();
}

void Tank::Render()
{
	if (!isActive)
		return;

	__super::Render();
	head->Render();

	collider->Render();

	curState->Render();
}

void Tank::PostRender()
{
	collider->RenderUI();
}

void Tank::CreateStates()
{
	auto patrol = new TankPatrol(this);
	patrol->SetAStar(astar);
	states[PATROL] = patrol;

	auto trace = new TankTrace(this);
	trace->SetAStar(astar);
	states[TRACE] = trace;
}

void Tank::SetState(int state)
{
	if (curState != states[(State)state]) {
		curState = states[(State)state];
		curState->Start();
	}
}

Vector2 Tank::GetFirePoint()
{
	return head->GlobalPos() + head->Right() * 30.0f;
}

void Tank::Control()
{
	if (KEY_PRESS('W')) Pos() += Right() * moveSpeed * DELTA;
	if(KEY_PRESS('S')) Pos() -= Right() * moveSpeed * DELTA;
	if (KEY_PRESS('A')) Rot().z += rotSpeed * DELTA;
	if(KEY_PRESS('D')) Rot().z -= rotSpeed * DELTA;
}

void Tank::HeadControl()
{

	Vector2 direction = mousePos - head->GlobalPos();


	head->Rot().z = direction.Angle() - Rot().z;
}

void Tank::MovePath()
{
	if (path.empty())
		return;

	Vector2 direction = path.back() - Pos();
	Rot().z = direction.Angle();

	Pos() += Right() * moveSpeed * DELTA;

	if (direction.Length() < 5.0f)
		path.pop_back();

}

void Tank::Fire()
{
	if (!isActive)
		return;

	Vector2 pos = GetFirePoint();
	BulletManager::Get()->Fire(pos, head->Right(), shooterTag);
}

void Tank::Destroy()
{
	isActive = false;

	if (destroyEvent)
		destroyEvent();
}
