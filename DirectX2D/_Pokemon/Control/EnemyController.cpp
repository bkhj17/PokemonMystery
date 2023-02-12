#include "Framework.h"
#include "EnemyController.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitManager.h"
#include "../Tile/DungeonTileMap.h"

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

bool EnemyController::UseSkill(int i)
{
	//��� 4�� + �⺻ ���� 5��

	if (i < 0 || i > 4)
		return false;

	//������ i��° ����� ����� �ϴ��� Ȯ��
	//unit; 

	//������ i��° ����� �ߵ������� �����ϴ��� Ȯ��

	//�����Ѵٸ� ����
	//��ų�� ��ٸ� true ��ȯ

	return false;
}

bool EnemyController::SetCommand()
{
	//���ʹ̴� SetCommand���� ��ų�̶� �Ϲ� ���������� �˻�
	//�Ϲ� ������ ��ų�� ����� ��!!!!
	//��ų 4���� �Ϲ� ������ �ȴ�

	return false;
}

void EnemyController::SetMoveCommand()
{
	//���ʹ̴� ������ �����δ� => �̵��Ѵٰ� ������ �ʿ䰡 ����
	if (unit->GetWait() != 0)
		return;


	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (tileMap == nullptr) {
		//�̻���� : �� �ν� �Ұ�
		return;
	}

	vector<pair<int, int>> detectables = tileMap->DetectableTiles(unit->GetPoint());
	//�÷��̾�, ���� ��ġ �޾ƿ���
	POINT uPoint = unit->GetPoint();
	POINT pPlayer = UnitManager::Get()->GetPlayer()->GetPoint();
	if (find(detectables.begin(), detectables.end(), pair<int, int>({ pPlayer.x, pPlayer.y })) != detectables.end()) {
		pair<int, int> next = tileMap->ChasingPoint({unit->GetPoint().x, unit->GetPoint().y}, {pPlayer.x, pPlayer.y});
		unit->SetMovePlan(next.first - uPoint.x, next.second - uPoint.y, 1);
		unit->SetDir(next.first - uPoint.x, next.second - uPoint.y);
		return;
	}
	
	SetPatrolMoveCommand();
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
