#pragma once
#include "Controller.h"

//���ᵵ ����� ������� ������ ���̴�
//��Ȳ ���� UseSkill�� ������ ����
class EnemyController : public Controller
{
public:
	EnemyController();
	EnemyController(Unit* unit);
	virtual ~EnemyController();

	// Controller��(��) ���� ��ӵ�
	virtual bool SetCommand() override;
	void SetMoveCommand();
private:
	virtual bool UseSkill(int i);
};

