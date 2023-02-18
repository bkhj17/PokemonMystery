#pragma once
//���ᵵ ����� ������� ������ ���̴�
//��Ȳ ���� UseSkill�� ������ ����
class EnemyController : public Controller
{
public:
	EnemyController();
	EnemyController(Unit* unit);
	virtual ~EnemyController() = default;

	virtual void Init() override;

	// Controller��(��) ���� ��ӵ�
	virtual bool SetCommand() override;
	bool ActivateReserved();
	void SetMoveCommand();

	void TurnEnd() override;
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
	int reservedSkill = -1;
	int transTime = 0;
	int transRate = 10;
};

