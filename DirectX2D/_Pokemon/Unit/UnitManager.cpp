#include "Framework.h"

UnitManager::UnitManager()
{
	LoadPokemonTable();
	LoadAnimData();
	LoadPokemonSkillTable();

	player = new Unit(new PlayerController(), UNIT_SIZE);
	/*
	//동료 생성 : 생략
	friends.resize(3);
	for (auto& f : friends) {
		f = new Unit(nullptr, UNIT_SIZE);
		f->SetActive(false);
	}
	*/
	enemies.resize(30);
	for (auto& e : enemies) {
		e = new Unit(new EnemyController(), UNIT_SIZE);
		e->SetActive(false);
	}

	Observer::Get()->AddEvent("PlayerCommand", [this]() {
		playerCommanded = true;
	});

	notShadeBuffer = new ColorBuffer;
	notShadeBuffer->Get() = { 1.0f, 0.0f, 1.0f, 1.0f };
}

UnitManager::~UnitManager()
{
	SAFE_DELETE(player);
	for (auto f : friends)
		delete f;
	friends.clear();
	friends.shrink_to_fit();

	for (auto e : enemies)
		delete e;
	enemies.clear();
	enemies.shrink_to_fit();

	delete notShadeBuffer;
}

void UnitManager::Init()
{
	player->SetData(1, 6);
}

void UnitManager::Update()
{
	player->Update();
	for (auto f : friends)
		f->Update();
	for (auto e : enemies)
		e->Update();
}

void UnitManager::Render()
{
	vector<Unit*> v;

	if (player->IsActing()) {
		string s = player->GetData()->statusData.name;
	}
	for (auto e : enemies) {
		if (e->Active() && CAM->ContainFrustum(e->GlobalPos(), e->GetSize()))
			v.push_back(e);
	}
	for (auto f : friends) {
		if (f->Active() && CAM->ContainFrustum(f->GlobalPos(), f->GetSize()))
			v.push_back(f);
	}
	v.push_back(player);

	sort(v.begin(), v.end(), [](Unit* l, Unit* r) {
		if (abs(abs(l->GlobalPos().y) - abs(r->GlobalPos().y)) < FLT_EPSILON)
			return !l->IsActing() && r->IsActing();
		return l->GlobalPos().y > r->GlobalPos().y;
	});

	notShadeBuffer->SetPS(1);
	for (auto unit : v)
		unit->Render();
	v.clear();
}

void UnitManager::RunPhase()
{
	if (IsActing())
		return;

	while (1) {
		switch (curPhase)
		{
		case UnitManager::PLAYER_COMMAND:
			if (player->GetWait() == 0) {
				if (playerCommanded || player->GetController()->SetCommand()) {
					player->GetWait() += 2;
					playerCommanded = false;
				}
				else {
					return;
				}
			}
			curPhase = PLAYER_ACT;
			break;
		case UnitManager::PLAYER_ACT:
			//이동 외에 다른걸 하는 중이면 리턴
			if (player->GetCurAction() > 1)
				return;

			curPhase = FRIEND_SKILL;
			break;
		case UnitManager::FRIEND_SKILL:
		{
			bool skillCall = false;
			for (auto f : friends) {
				if (!f->Active())
					continue;
				//행동권 존재
				if (f->GetWait() == 0) {
					//만약 스킬을 쓴다면
					if (false) {
						//시전자의 턴을 증가시키고 리턴
						f->GetWait() += 2;
						return;
					}
				}
			}
			curPhase = FRIEND_MOVE;
		}
		break;
		case UnitManager::FRIEND_MOVE:
			//이동의 경우 전원이 동시에 판단
			for (auto f : friends) {
				if (!f->Active())
					continue;
			}
			curPhase = ENEMY_SKILL;
		case UnitManager::ENEMY_SKILL:
		{
			bool skillCall = false;
			for (auto e : enemies) {
				//유효하지 않음
				if (!e->Active())
					continue;

				//예약된 스킬이 있다면 사용......
				auto eController = (EnemyController*)e->GetController();
				if (eController->ActivateReserved())
					return;

				//행동권 존재하고 만약 스킬을 쓴다면 
				if (e->GetWait() == 0 && eController->SetCommand()) {
					//시전자의 턴을 증가시키고 리턴
					e->GetWait() += 2;
					skillCall = true;
					return;
				}
			}
			curPhase = ENEMY_MOVE;
		}
		break;
		case UnitManager::ENEMY_MOVE:
			//이동의 경우 전원이 동시에 판단
			for (auto e : enemies) {
				if (e->GetWait() > 0)
					continue;

				auto controller = (EnemyController*)e->GetController();
				controller->SetMoveCommand();
				e->GetWait() += 2;
			}

			curPhase = TURN_END;
			return;
		case UnitManager::TURN_END:
			//모든 유닛이 턴 감소
			player->TurnEnd();
			for (auto f : friends)
				if (f->Active())
					f->TurnEnd();
			for (auto e : enemies) {
				if (e->Active())
					e->TurnEnd();
			}
			curPhase = PLAYER_COMMAND;
			return;
		default:
			return;
		}
	}
}

bool UnitManager::IsActing()
{
	bool isActing = false;
	if (player->IsActing())
		return true;

	for (auto f : friends)
		if (f->IsActing())
			return true;
	for (auto e : enemies)
		if (e->IsActing())
			return true;

	return false;
}

void UnitManager::GetPokemonData(IN int key, IN int level, OUT PokemonData*& data)
{
	if (data == nullptr)
		data = new PokemonData;

	data->key = key;
	data->level = level;
	ApplyLevel(level, data);
}

void UnitManager::GetInitSkills(IN int key, IN int level, OUT vector<int>& result)
{
	if (pokemonSkillTable.find(key) == pokemonSkillTable.end())
		return;

	stack<int> st;
	auto& mp = pokemonSkillTable[key];
	for (auto& skillKey : mp) {
		if (skillKey.first > level)
			break;

		for (auto key : skillKey.second)
			st.push(key);
	}

	for (int i = 0; i < 4 && !st.empty(); i++) {
		result.push_back(st.top());
		st.pop();
	}

	reverse(result.begin(), result.end());
	return;
}

void UnitManager::GetSkillDataKey(IN int key, IN int level, OUT vector<int>& v)
{
	if (pokemonSkillTable.find(key) == pokemonSkillTable.end())
		return;

	if (pokemonSkillTable[key].find(level) == pokemonSkillTable[key].end())
		return;

	v.insert(v.end(), pokemonSkillTable[key][level].begin(), pokemonSkillTable[key][level].end());
}

bool UnitManager::IsUnitOnPoint(POINT point)
{
	POINT p = player->GetPoint();

	if (p.x == point.x && p.y == point.y)
		return true;

	for (auto f : friends) {
		if (!f->Active())
			continue;
		p = f->GetPoint();
		if (p.x == point.x && p.y == point.y)
			return true;
	}

	for (auto e : enemies)
	{
		if (!e->Active())
			continue;
		p = e->GetPoint();
		if (p.x == point.x && p.y == point.y)
			return true;
	}

	return false;
}

Unit* UnitManager::GetUnitOnPoint(POINT point)
{

	POINT p = player->GetPoint();
	if (p.x == point.x && p.y == point.y)
		return player;

	for (auto f : friends) {
		p = f->GetPoint();
		if (p.x == point.x && p.y == point.y)
			return f;
	}

	for (auto e : enemies)
	{
		p = e->GetPoint();
		if (p.x == point.x && p.y == point.y)
			return e;
	}

	return nullptr;
}

void UnitManager::ClearEnemy()
{
	for (auto e : enemies)
		e->SetActive(false);
}

void UnitManager::InitEnemy() {
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (!tileMap)
		return;

	auto& monsters = tileMap->GetFloorData()->monsters;
	int i = Random(0, (int)monsters.size());
	InitEnemy(monsters[i].first, monsters[i].second);
}

void UnitManager::InitEnemy(int key, int level)
{
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (!tileMap)
		return;

	Unit* unit = nullptr;
	int cnt = 0;
	for (auto e : enemies) {
		if (e->Active())
			cnt++;
		else if (unit == nullptr)
			unit = e;
	}

	if (tileMap->GetFloorData()->monsterNum <= cnt)
		return;

	POINT point = tileMap->GetRandomPointByCondition([this, tileMap](POINT point) -> bool {
		if (CAM->ContainFrustum(tileMap->PointToPos(point), UNIT_SIZE))
			return false;
		
		auto tile = tileMap->GetBgTile(point);
		if (tile == nullptr)
			return false;
		if (BgTileManager::Get()->GetLandTexture() != tile->GetTexture())
			return false;
		if (!BgTileManager::Get()->IsRoom(tile->GetGridFlag()))
			return false;
		if (GetUnitOnPoint(point))
			return false;
		return true;
	});

	if (point.x == -1 || point.y == -1)
		return;

	if (tileMap->GetBgTile(point)->GetTexture() != BgTileManager::Get()->GetLandTexture())
		return;

	unit->SetData(key, level);
	unit->SetPoint(point);
	unit->Init();
}

bool UnitManager::CheckMovablePoint(POINT point, int dirX, int dirY)
{
	//맵 정보 불러올 수 있는지 확인
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (tileMap == nullptr)
		return false;

	Vector2 destPos = {};	//함수 호출용 더미
	//갈 수 있는 위치인지 확인
	if (!tileMap->SetMove(point.x, point.y, dirX, dirY, destPos))
		return false;

	POINT nextPos = point;
	nextPos.x += dirX;
	nextPos.y += dirY;

	if (tileMap->GetBgTile(nextPos)->GetTexture() == BgTileManager::Get()->GetWallTexture()) {
		return false;
	}

	//가야 할 자리에 누가 먼저 자리잡고 있는지도 알아야 함
	if (IsUnitOnPoint(nextPos))
		return false;

	return true;
}

void UnitManager::ApplyLevel(IN int level, OUT PokemonData* data)
{
	//기본 종족값 데이터 복사
	PokemonTableData tableData = pokemonTable[data->key];
	//레벨 반영
	tableData.maxHp = PokemonTableData::ApplyLevelHp(level, tableData.maxHp);
	tableData.attack = PokemonTableData::ApplyLevel(level, tableData.attack);
	tableData.defense = PokemonTableData::ApplyLevel(level, tableData.defense);
	tableData.sAttack = PokemonTableData::ApplyLevel(level, tableData.sAttack);
	tableData.sDefense = PokemonTableData::ApplyLevel(level, tableData.sDefense);
	tableData.speed = PokemonTableData::ApplyLevel(level, tableData.speed);

	float hpRate = (float)data->curHp / data->statusData.maxHp;
	data->level = level;
	data->statusData = tableData;
	data->curHp = (int)ceil(hpRate * data->statusData.maxHp);
}

void UnitManager::LoadPokemonTable()
{
	ifstream ifs;
	ifs.open(POKEMON_TABLE);

	if (ifs.fail())
		//로드 실패
		return;

	bool isFirst = true;
	while (!ifs.eof()) {
		string tmp;
		getline(ifs, tmp);
		if (isFirst) {
			isFirst = false;
			continue;
		}

		if (tmp.empty())
			break;

		vector<string> v = SplitString(tmp, ",");
		PokemonTableData data;
		data.key = stoi(v[0]);
		data.name = v[1];
		vector<string> types = SplitString(v[2], " ");
		for (auto& type : types)
			data.type.push_back((PokemonType)stoi(type));

		data.maxHp = stoi(v[3]);
		data.attack = stoi(v[4]);
		data.defense = stoi(v[5]);
		data.sAttack = stoi(v[6]);
		data.sDefense = stoi(v[7]);
		data.speed = stoi(v[8]);

		pokemonTable[data.key] = data;
	}
}

void UnitManager::LoadAnimData()
{	
	AnimData data;
	data.pokemonNum = 1;
	data.textureFile = L"Textures/pokemon/이상해씨.png";
	data.texcoord = Vector2(10, 8);
	{
		//LeftUp : 0
		int dirCode = 0;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, 3 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, 3 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, 3 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, 3 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 3 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 3 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 3 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 3 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 3 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 3 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 3 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 3 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 3 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 3 });

		//Up : 1
		dirCode = 1;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, 4 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, 4 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, 4 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, 4 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 4 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 4 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 4 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 4 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 4 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 4 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 4 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 4 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 4 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 4 });

		//RightUp : 2
		dirCode = 2;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, 5 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, 5 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, 5 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, 5 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 5 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 5 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 5 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 5 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 5 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 5 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 5 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 5 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 5 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 5 });

		//Left : 3
		dirCode = 3;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, 2 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, 2 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, 2 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, 2 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 2 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 2 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 2 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 2 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 2 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 2 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 2 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 2 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 2 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 2 });

		//4는 중앙 => 계산에 좋지 않아 쓰지 않는다

		//Right : 5
		dirCode = 5;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, 6 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, 6 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, 6 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, 6 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 6 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 6 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 6 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 6 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 6 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 6 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 6 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 6 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 6 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 6 });

		//LeftDown : 6
		dirCode = 6;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, 1 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, 1 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, 1 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, 1 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 1 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 1 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 1 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 1 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 1 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 1 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 1 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 1 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 1 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 1 });

		//Down : 7, default
		dirCode = 7;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, 0 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, 0 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, 0 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, 0 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 0 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 0 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 0 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 0 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 0 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 0 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 0 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 0 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 0 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 0 });

		//RightDown : 8
		dirCode = 8;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, 7 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, 7 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, 7 });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, 7 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 7 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 3, 7 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 7 });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, 7 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 7 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 5, 7 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 7 });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 6, 7 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 7 });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 7, 7 });
	}
	animTable[data.pokemonNum] = data;


////////////////////////////////////////////////////////////////////////////////////////////////////////

	data.pokemonNum = 19;
	data.textureFile = L"Textures/pokemon/꼬렛.png";
	data.texcoord = { 12, 8 };
	data.clipData.clear();
	{
		//LeftUp : 0
		int dirCode = 0;
		int inTex = 3;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 3, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 5, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 6, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 7, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 8, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 9, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 10, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });

		//Up : 1
		dirCode = 1;
		inTex = 4;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 3, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 5, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 6, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 7, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 8, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 9, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 10, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });

		//RightUp : 2
		dirCode = 2;
		inTex = 5;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 3, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 5, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 6, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 7, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 8, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 9, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 10, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });

		//Left : 3
		dirCode = 3;
		inTex = 2;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 3, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 5, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 6, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 7, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 8, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 9, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 10, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });

		//4는 중앙 => 계산에 좋지 않아 쓰지 않는다

		//Right : 5
		dirCode = 5;
		inTex = 6;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 3, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 5, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 6, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 7, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 8, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 9, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 10, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });

		//LeftDown : 6
		dirCode = 6;
		inTex = 1;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 3, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 5, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 6, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 7, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 8, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 9, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 10, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });

		//Down : 7, default
		dirCode = 7;
		inTex = 0;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 3, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 5, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 6, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 7, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 8, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 9, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 10, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });

		//RightDown : 8
		dirCode = 8;
		inTex = 7;
		data.clipData[dirCode * 100 + Unit::IDLE].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 0, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 1, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 2, inTex });
		data.clipData[dirCode * 100 + Unit::MOVING].push_back({ 3, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 4, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 5, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_PHYSICS].push_back({ 6, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 7, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 8, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 9, inTex });
		data.clipData[dirCode * 100 + Unit::SKILL_SPECIAL].push_back({ 10, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
		data.clipData[dirCode * 100 + Unit::DAMAGE].push_back({ 11, inTex });
	}
	animTable[data.pokemonNum] = data;
}

void UnitManager::LoadPokemonSkillTable() {
	ifstream ifs;
	ifs.open(POKEMON_SKILL_TABLE);
	if (ifs.fail())
		return;

	bool isFirst = true;
	while (!ifs.eof()) {
		string tmp;
		getline(ifs, tmp);
		if (isFirst) {
			isFirst = false;
			continue;
		}

		if (tmp.empty())
			continue;

		vector<string> v = SplitString(tmp, ",");
		int pokemon = stoi(v[0]);
		int level = stoi(v[1]);
		int skill = stoi(v[2]);

		pokemonSkillTable[pokemon][level].push_back(skill);
	}
}