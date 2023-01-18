#include "Framework.h"
#include "Robot0118.h"
#include "../Command0118.h"
#include "Objects/Character/Ninja/NinjaRun.h"
#include "RobotJumpUp0118.h"
#include "RobotJumpDown0118.h"
#include "RobotMeleeAttack0118.h"
#include "RobotJumpAttack0118.h"
#include "RobotShoot0118.h"

Robot0118::Robot0118()
{
	Pos() = { CENTER_X, 200.0f };

	CreateActions();
	CreateCommands();
}

Robot0118::~Robot0118()
{
	for (auto& action : actions)
		delete action.second;
}

void Robot0118::Update()
{
	Control();
	actions[curAction]->Update();

	UpdateWorld();
}

void Robot0118::Render()
{
	SetRender();
	actions[curAction]->Render();
}

void Robot0118::CreateActions()
{
	string path = "Textures/Robot/";
	actions[IDLE] = new Action(path, "Idle.xml", true);
	actions[RUN] = new NinjaRun(this, path, "Run.xml");
	actions[JUMP_UP] = new RobotJumpUp0118(this);
	actions[JUMP_UP]->SetEvent(bind(&Robot0118::SetAction, this, JUMP_DOWN));
	actions[JUMP_DOWN] = new RobotJumpDown0118(this);
	actions[JUMP_DOWN]->SetEvent(bind(&Robot0118::SetAction, this, IDLE));
	
	actions[MELEE] = new RobotMeleeAttack0118(this);
	actions[MELEE]->SetEvent(bind(&Robot0118::SetAction, this, IDLE));
	actions[JUMP_MELEE] = new RobotJumpAttack0118(this);
	actions[JUMP_MELEE]->SetEvent(bind(&Robot0118::SetAction, this, JUMP_DOWN));

	actions[SHOOT] = new RobotShoot0118(this);
	actions[SHOOT]->SetEvent(bind(&Robot0118::SetAction, this, IDLE));

	actions[SPECIAL_SHOOT] = new RobotShoot0118(this);
	actions[SPECIAL_SHOOT]->SetEvent(bind(&Robot0118::SetAction, this, IDLE));

	auto specialShoot = (RobotShoot0118*)actions[SPECIAL_SHOOT];
	specialShoot->SetFireFunc([this, specialShoot]() {
		Vector2 firePos = this->Pos() + Right() * 20.0f;
		BulletManager::Get()->Fire(firePos, Right());
		BulletManager::Get()->Fire(firePos, (Up() + Right()).GetNormalized());
		BulletManager::Get()->Fire(firePos, (Down() + Right()).GetNormalized());
	});
}

void Robot0118::CreateCommands()
{
	vector<UINT> v = { VK_DOWN, VK_LEFT, 'X' };
	Command0118::Get()->SetCommand("SpecialShootLeft", v);

	v.clear();
	v = { VK_DOWN, VK_RIGHT, 'X' };
	Command0118::Get()->SetCommand("SpecialShootRight", v);
}

void Robot0118::Control()
{
	InputCommand();
	SpecialShoot();

	if (curAction == SPECIAL_SHOOT)
		return;

	Jump();
	Move();
	Attack();
}

void Robot0118::SpecialShoot()
{
	if (Command0118::Get()->LastCommand() == "SpecialShootLeft"
		|| Command0118::Get()->LastCommand() == "SpecialShootRight") {
		Command0118::Get()->ClearInputs();
		if (curAction <= RUN)
			SetAction(SPECIAL_SHOOT);
	}
}

void Robot0118::Move()
{
	if (curAction >= JUMP_UP)
		return;

	bool isMove = false;

	isMove |= KEY_PRESS(VK_RIGHT);
	isMove |= KEY_PRESS(VK_LEFT);

	SetAction(isMove ? RUN : IDLE);
}

void Robot0118::Attack()
{
	if (curAction >= MELEE)
		return;

	if (KEY_DOWN('Z')) {
		if (curAction == JUMP_UP || curAction == JUMP_DOWN)
			SetAction(JUMP_MELEE);
		else
			SetAction(MELEE);
	}

	if (KEY_DOWN('X')) {
		if (curAction == JUMP_UP || curAction == JUMP_DOWN) {

		}
		else
			SetAction(SHOOT);
	}
}

void Robot0118::Jump()
{

	if (curAction == JUMP_UP || curAction == JUMP_DOWN) return;
	if (curAction >= MELEE) 
		return;

	if (KEY_DOWN(VK_UP))
		SetAction(JUMP_UP);
}

void Robot0118::InputCommand()
{

	if (KEY_DOWN(VK_LEFT))
		Command0118::Get()->InputCommand(VK_LEFT);

	if (KEY_DOWN(VK_RIGHT))
		Command0118::Get()->InputCommand(VK_RIGHT);

	if (KEY_DOWN(VK_UP))
		Command0118::Get()->InputCommand(VK_UP);

	if (KEY_DOWN(VK_DOWN))
		Command0118::Get()->InputCommand(VK_DOWN);

	if (KEY_DOWN('Z'))
		Command0118::Get()->InputCommand('Z');
	if (KEY_DOWN('X'))
		Command0118::Get()->InputCommand('X');
}

void Robot0118::SetAction(ActionType type)
{
	if (curAction == type)
		return;

	curAction = type;
	actions[type]->Start();
}
