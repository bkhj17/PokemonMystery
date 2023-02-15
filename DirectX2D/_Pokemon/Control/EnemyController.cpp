#include "Framework.h"
#include "EnemyController.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitManager.h"
#include "../Tile/DungeonTileMap.h"
#include "../Skill/Skill.h"
#include "../Data/SkillDataManager.h"

EnemyController::EnemyController()
{
	tag = "Enemy";
	controllerType = ControllerType::ENEMY;
}

EnemyController::EnemyController(Unit* unit)
	: Controller(unit)
{
	tag = "Enemy";
	controllerType = ControllerType::ENEMY;
}

void EnemyController::Init()
{
	reservedSkill = -1;
}

bool EnemyController::SetCommand()
{
	//���ʹ̴� SetCommand���� ��ų�̶� �Ϲ� ���������� �˻�
	//�Ϲ� ������ ��ų�� ����� ��!!!!
	//��ų 0���� �Ϲ� ������ �ȴ�

	//���� Ÿ���� �� �� �ִ� ��� ������ ������ ���
	//��� ��ȿ ���ε� Ȯ���ؼ�
	//��� ��� �����ϰ� ���� true
	if (!unit->Active())
		return false;

	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (tileMap == nullptr)	//�̻���� : �� �ν� �Ұ�
		return false;

	//�νĹ���
	vector<pair<int, int>> detectables = tileMap->DetectableTiles(unit->GetPoint());
	//�÷��̾�, ���� ��ġ �޾ƿ���

	POINT pTarget = ChooseTarget(detectables);
	if (pTarget.x == -1 || pTarget.y == -1)
		return false;

	for (int i = 1; i <= 5; i++) {
		//��ų ��� ������ �����ϴ��� �˾ƾ� ��
		if (!unit->skills[i % 5]->IsValid())
			continue;

		//��ȭ��� : ���� ���� ������ �ʿ������� �ð� ����� 5�� ������ �� �� �ִ� �ɷ� 
		SkillData::SkillType skillType = unit->skills[i % 5]->GetData()->type;
		if (skillType == SkillData::TRANS && transTime > 0)
			continue;
		if(!unit->skills[i%5]->IsInRange(unit->GetPoint(), pTarget))
			continue;
		unit->SetDir(
			max(-1, min(1, pTarget.x - unit->GetPoint().x)),
			max(-1, min(1, pTarget.y - unit->GetPoint().y))
		);
		if (unit->IsUsableSkill(i % 5)) {
			if (skillType == SkillData::TRANS)
				transTime = transRate;
			reservedSkill = i % 5;
			return true;
		}
	}

	return false;
}

bool EnemyController::ActivateReserved()
{
	if (reservedSkill >= 0 && reservedSkill < 5) {
		unit->UseSkill(reservedSkill);
		reservedSkill = -1;
		return true;
	}
	return false;
}

void EnemyController::SetMoveCommand()
{
	if (!unit->Active())
		return;

	//���ʹ̴� ������ �����δ� => �̵��Ѵٰ� ������ �ʿ䰡 ����
	if (unit->GetWait() != 0)
		return;

	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (tileMap == nullptr)	//�̻���� : �� �ν� �Ұ�
		return;

	//�νĹ���
	vector<pair<int, int>> detectables = tileMap->DetectableTiles(unit->GetPoint());
	//�÷��̾�, ���� ��ġ �޾ƿ���

	POINT pTarget = ChooseTarget(detectables);

	//Ÿ�� �� �ν� ���� �ȿ� ���� �༮�� ������
	if (pTarget.x < 0 || pTarget.y < 0) {
		//���ƴٴϱ�
		SetPatrolMoveCommand();
		return;
	}

	//Ÿ�� �߰� �� �ѱ�
	ChaseMoveCommand(detectables, make_pair(pTarget.x, pTarget.y));
}

void EnemyController::TurnEnd()
{
	transTime = max(--transTime, 0);
}

POINT EnemyController::ChooseTarget(vector<pair<int, int>>& detectables)
{
	POINT uPoint = unit->GetPoint();
	vector<POINT> vTargets;
	vTargets.push_back(UnitManager::Get()->GetPlayer()->GetPoint());
	for (auto f : UnitManager::Get()->GetFriends())
		vTargets.push_back(f->GetPoint());

	POINT pTarget = { -1, -1 };
	int minDist = 100000000;
	//Ÿ�� ��ġ�� �˻��ؼ� ���� ����� �༮�� �޴´�
	for (const auto& target : vTargets) {
		if (find(detectables.begin(), detectables.end(), make_pair((int)target.x, (int)target.y)) == detectables.end())
			continue;

		int tDist = abs(target.x - uPoint.x) + abs(target.y - uPoint.y);
		if (minDist > tDist) {
			minDist = tDist;
			pTarget = target;
		}
	}
	return pTarget;
}

void EnemyController::ChaseMoveCommand(vector<pair<int, int>>& detectables, pair<int, int> target)
{
	//�ڽ� ��ġ
	POINT uPoint = unit->GetPoint();

	pair<int, int> start = make_pair(uPoint.x, uPoint.y);
	map<pair<int, int>, ChaseNode> points;
	priority_queue<ChaseNode> pq;
	pq.push({ start, start, 0 });

	POINT dir[8] = {
		{-1, 1}, //leftUp
		{0, 1}, //up
		{1, 1}, //rightUp
		{-1, 0}, //left
		{1, 0}, //right
		{-1, -1}, //leftDown
		{0, -1}, //down
		{1, -1}, //rightDown
	};

	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	Vector2 destPos = {};	//�Լ� ȣ��� ����
	
	pair<int, int> lastPoint = start;
	while (!pq.empty()) {
		ChaseNode curNode = pq.top();
		pq.pop();

		if (points.find(curNode.point) != points.end()) {
			if (points[curNode.point].dist <= curNode.dist)
				continue;
		}

		points[curNode.point] = curNode;
		lastPoint = make_pair(curNode.point.first, curNode.point.second);
		if (curNode.point == target)
			break;

		for (int i = 0; i < 8; i++) {
			//�� �� ���� ��ġ��� �ѱ��
			if (!tileMap->SetMove(curNode.point.first, curNode.point.second, dir[i].x, dir[i].y, destPos))
				continue;

			pair<int, int> nextPoint = make_pair(curNode.point.first + dir[i].x, curNode.point.second + dir[i].y);
			//ã�� �� ���� ��ġ���� �ѱ��
			if (find(detectables.begin(), detectables.end(), nextPoint) == detectables.end())
				continue;

			//Ÿ�� �̿ܿ� �ٸ� ������ �ִ��� Ȯ��
			if (nextPoint != target && UnitManager::Get()->IsUnitOnPoint({ nextPoint.first, nextPoint.second }))
				continue;
						
			ChaseNode nextNode;
			nextNode.point = nextPoint;
			nextNode.post = curNode.point;
			nextNode.dist = curNode.dist + 1;

			pq.push(nextNode);
		}
	}

	//lastPoint�� �������� ������ - ���� �������� ���� ���� ���� ã�´�
	while (points[lastPoint].post != start) {
		lastPoint = points[lastPoint].post;
	}

	//ã�Ƴ� ������ ���� �̵�
	int dx = lastPoint.first - uPoint.x;
	int dy = lastPoint.second - uPoint.y;
	if (max(abs(dx), abs(dy)) > 1) //�̻����
		return;

	if (lastPoint != target)
		unit->SetMovePlan(dx, dy, 1);
	unit->SetDir(dx, dy);
}

void EnemyController::SetPatrolMoveCommand()
{
	////���� ����ġ ���
	int dirX = unit->GetDirX();
	int dirY = unit->GetDirY();

	pair<POINT, int> points[] = {
		{{-1, -1}, 0 },
		{{0, -1}, 0 },
		{{1, -1}, 0 },
		{{-1, 0}, 0 },
		{{1, 0}, 0 },
		{{-1, 1}, 0 },
		{{0, 1}, 0 },
		{{1, 1}, 0 },
	};

	//����ġ ���
	for (int i = 0; i < 8; i++) {
		//x�� dirX�� ��ġ Ȥ�� dirX�� 0�̸� ����ġ 1 �߰�
		if (points[i].first.x == -dirX && points[i].first.y == -dirY) {
			points[i].second = -1;
			continue;
		}

		if (points[i].first.x == dirX)
			points[i].second++;

		//y�� dirY�� ��ġ Ȥ�� dirY�� 0�̸� ����ġ 1 �߰�
		if (points[i].first.y == dirY)
			points[i].second++;
	}
	//����ġ ������ ����
	sort(points, points + 8, [](const pair<POINT, int>& l, const pair<POINT, int>& r) -> bool {
		return l.second > r.second;
	});

	POINT curPoint = unit->GetPoint();
	for (int i = 0; i < 8; i++) {
		POINT dir = points[i].first;
		if (!UnitManager::Get()->CheckMovablePoint(curPoint, dir.x, dir.y))
			continue;
		unit->SetMovePlan(dir.x, dir.y, 1);
		unit->SetDir(dir.x, dir.y);
		break;
	}
}
