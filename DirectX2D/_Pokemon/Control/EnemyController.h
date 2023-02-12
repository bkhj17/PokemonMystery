#pragma once
#include "Controller.h"

//���ᵵ ����� ������� ������ ���̴�
//��Ȳ ���� UseSkill�� ������ ����
class EnemyController : public Controller
{
public:
	EnemyController();
	EnemyController(Unit* unit);
	virtual ~EnemyController() = default;

	// Controller��(��) ���� ��ӵ�
	virtual bool SetCommand() override;
	void SetMoveCommand();

private:
	void SetPatrolMoveCommand();

private:
	virtual bool UseSkill(int i);
};

