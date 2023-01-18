#include "Framework.h"
#include "NinjaIdle.h"
#include "NinjaRun.h"
#include "NinjaJump.h"
#include "NinjaMeleeAttack.h"
#include "NinjaJumpAttack.h"
#include "NinjaThrow.h"

Ninja::Ninja()
{
	Pos() = { CENTER_X, CENTER_Y };
	CreateActions();

	curAction = IDLE;
	actions[curAction]->Start();
}

Ninja::~Ninja()
{
	for (auto& action : actions)
		delete action.second;
}

void Ninja::Update()
{
	Control();
	actions[curAction]->Update();

	UpdateWorld();
}

void Ninja::Render()
{
	SetRender();
	actions[curAction]->Render();
}

void Ninja::Control()
{
	Jump();
	Move();
	Attack();
}

void Ninja::Move()
{
	if (curAction == JUMP || curAction >= ATTACK) 
		return;
	
	bool isMove = false;

	isMove |= KEY_PRESS(VK_RIGHT);
	isMove |= KEY_PRESS(VK_LEFT);

	SetAction(isMove ? RUN : IDLE);
}

void Ninja::Attack()
{
	if (curAction >= ATTACK)
		return;

	if (KEY_DOWN('Z')) {

		if (curAction == JUMP)
			SetAction(JUMP_ATTACK);
		else
			SetAction(ATTACK);
	}

	if (KEY_DOWN('S')) {

		if (curAction == JUMP)
			SetAction(JUMP_ATTACK);
		else
			SetAction(THROW);
	}
}

void Ninja::Jump()
{
	if (curAction == JUMP) return;
	if(curAction >= ATTACK)
		return;

	if (KEY_DOWN(VK_SPACE)) {
		SetAction(JUMP);
	}
}

void Ninja::Land()
{
	SetAction(IDLE);
}

void Ninja::EndAttack()
{
	SetAction(IDLE);
}

void Ninja::CreateActions()
{
	actions[IDLE] = new Action("Textures/Ninja/", "Ninja_Idle.xml", true);
	actions[RUN] = new NinjaRun(this);
	actions[JUMP] = new NinjaJump(this);
	actions[JUMP]->SetEvent(bind(&Ninja::SetIdle, this));
	actions[ATTACK] = new NinjaMeleeAttack(this);
	actions[ATTACK]->SetEvent(bind(&Ninja::SetIdle, this));
	actions[JUMP_ATTACK] = new NinjaJumpAttack(this);
	actions[JUMP_ATTACK]->SetEvent(bind(&Ninja::SetIdle, this));

	actions[THROW] = new NinjaThrow(this);
	actions[THROW]->SetEvent(bind(&Ninja::SetIdle, this));
}

void Ninja::SetAction(ActionType type)
{
	if (curAction == type)
		return;

	curAction = type;
	actions[type]->Start();
}
