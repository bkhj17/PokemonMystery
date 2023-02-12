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
	//기술 4개 + 기본 공격 5개

	if (i < 0 || i > 4)
		return false;

	//유닛의 i번째 기술이 존재는 하는지 확인
	//unit; 

	//유닛의 i번째 기술이 발동조건을 만족하는지 확인

	//만족한다면 쓰자
	//스킬을 썼다면 true 반환

	return false;
}

bool EnemyController::SetCommand()
{
	//에너미는 SetCommand에서 스킬이랑 일반 공격할지만 검사
	//일반 공격을 스킬로 만들면 돼!!!!
	//스킬 4번이 일반 공격이 된다

	return false;
}

void EnemyController::SetMoveCommand()
{
	//에너미는 일제히 움직인다 => 이동한다고 선언할 필요가 없음
	if (unit->GetWait() != 0)
		return;


	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (tileMap == nullptr) {
		//이상사태 : 맵 인식 불가
		return;
	}

	vector<pair<int, int>> detectables = tileMap->DetectableTiles(unit->GetPoint());
	//플레이어, 동료 위치 받아오기
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
	////방향 가중치 사용
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

	//가중치 계산
	for (int i = 0; i < 8; i++) {
		//x가 dirX와 일치 혹은 dirX가 0이면 가중치 1 추가
		if (points[i].first.x == -dirX && points[i].first.y == -dirY) {
			points[i].second = -1;
			continue;
		}

		if (points[i].first.x == dirX)
			points[i].second++;

		//y가 dirY와 일치 혹은 dirY가 0이면 가중치 1 추가
		if (points[i].first.y == dirY)
			points[i].second++;
	}
	//가중치 순으로 정렬
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
