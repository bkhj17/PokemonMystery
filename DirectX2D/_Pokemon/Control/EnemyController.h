#pragma once
#include "Controller.h"

//동료도 비슷한 방식으로 움직일 것이다
//상황 봐서 UseSkill도 밖으로 빼자
class EnemyController : public Controller
{
public:
	EnemyController();
	EnemyController(Unit* unit);
	virtual ~EnemyController() = default;

	// Controller을(를) 통해 상속됨
	virtual bool SetCommand() override;
	void SetMoveCommand();

private:
	void SetPatrolMoveCommand();

private:
	virtual bool UseSkill(int i);
};

