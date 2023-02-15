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
	//에너미는 SetCommand에서 스킬이랑 일반 공격할지만 검사
	//일반 공격을 스킬로 만들면 돼!!!!
	//스킬 0번이 일반 공격이 된다

	//만약 타겟이 쓸 수 있는 기술 범위에 들어왔을 경우
	//기술 유효 여부도 확인해서
	//기술 사용 예약하고 리턴 true
	if (!unit->Active())
		return false;

	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (tileMap == nullptr)	//이상사태 : 맵 인식 불가
		return false;

	//인식범위
	vector<pair<int, int>> detectables = tileMap->DetectableTiles(unit->GetPoint());
	//플레이어, 동료 위치 받아오기

	POINT pTarget = ChooseTarget(detectables);
	if (pTarget.x == -1 || pTarget.y == -1)
		return false;

	for (int i = 1; i <= 5; i++) {
		//스킬 사용 조건을 충족하는지 알아야 함
		if (!unit->skills[i % 5]->IsValid())
			continue;

		//변화기술 : 원래 개별 조건이 필요하지만 시간 관계상 5턴 지나야 쓸 수 있는 걸로 
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

	//에너미는 일제히 움직인다 => 이동한다고 선언할 필요가 없음
	if (unit->GetWait() != 0)
		return;

	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (tileMap == nullptr)	//이상사태 : 맵 인식 불가
		return;

	//인식범위
	vector<pair<int, int>> detectables = tileMap->DetectableTiles(unit->GetPoint());
	//플레이어, 동료 위치 받아오기

	POINT pTarget = ChooseTarget(detectables);

	//타겟 중 인식 범위 안에 들어온 녀석이 없었다
	if (pTarget.x < 0 || pTarget.y < 0) {
		//돌아다니기
		SetPatrolMoveCommand();
		return;
	}

	//타겟 발견 시 쫓기
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
	//타겟 위치를 검사해서 가장 가까운 녀석을 받는다
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
	//자신 위치
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
	Vector2 destPos = {};	//함수 호출용 더미
	
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
			//갈 수 없는 위치라면 넘긴다
			if (!tileMap->SetMove(curNode.point.first, curNode.point.second, dir[i].x, dir[i].y, destPos))
				continue;

			pair<int, int> nextPoint = make_pair(curNode.point.first + dir[i].x, curNode.point.second + dir[i].y);
			//찾을 수 없는 위치여도 넘긴다
			if (find(detectables.begin(), detectables.end(), nextPoint) == detectables.end())
				continue;

			//타겟 이외에 다른 유닛이 있는지 확인
			if (nextPoint != target && UnitManager::Get()->IsUnitOnPoint({ nextPoint.first, nextPoint.second }))
				continue;
						
			ChaseNode nextNode;
			nextNode.point = nextPoint;
			nextNode.post = curNode.point;
			nextNode.dist = curNode.dist + 1;

			pq.push(nextNode);
		}
	}

	//lastPoint를 기점으로 역추적 - 현재 지점에서 어디로 가야 할지 찾는다
	while (points[lastPoint].post != start) {
		lastPoint = points[lastPoint].post;
	}

	//찾아낸 지점을 향해 이동
	int dx = lastPoint.first - uPoint.x;
	int dy = lastPoint.second - uPoint.y;
	if (max(abs(dx), abs(dy)) > 1) //이상사태
		return;

	if (lastPoint != target)
		unit->SetMovePlan(dx, dy, 1);
	unit->SetDir(dx, dy);
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
