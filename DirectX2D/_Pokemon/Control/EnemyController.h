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

protected:
	POINT ChooseTarget(vector<pair<int, int>>& detectables);

	struct ChaseNode {
		pair<int, int> point = { -1, -1 };
		pair<int, int> post = { -1, -1 };
		int dist = 1000000000;

		bool operator<(const ChaseNode& node) const {
			return dist > node.dist;
		}
	};
	void ChaseMoveCommand(vector<pair<int, int>>& detectables, pair<int, int> target);
	void SetPatrolMoveCommand();

protected:
	virtual bool UseSkill(int i);
};

