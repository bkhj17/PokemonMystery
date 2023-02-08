#include "Framework.h"
#include "UnitManager.h"
#include "Unit.h"
#include "../Control/PlayerController.h"
#include "../Control/EnemyController.h"

UnitManager::UnitManager()
{
	LoadPokemonTable("TextData/PokemonData.csv");

	player = new Unit(new PlayerController(), Vector2(80, 80));

	friends.reserve(4);
	enemies.reserve(100);
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
}

void UnitManager::Init()
{
	player->SetData(1, 5);

	Unit* testEnemy = new Unit(new EnemyController(), Vector2(80, 80));
	testEnemy->SetData(1, 5);

	enemies.push_back(testEnemy);
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
	player->Render();
	for (auto f : friends)
		f->Render();
	for (auto e : enemies)
		e->Render();
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
				if (player->GetController()->SetCommand())
					player->GetWait() += 2;
				else
					return;
			}
			curPhase = PLAYER_ACT;
			break;
		case UnitManager::PLAYER_ACT:
			curPhase = FRIEND_SKILL;
			//이동 외에 다른걸 하는 중이면 리턴
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
					//시전자의 턴을 증가시키고 
					//리턴
				}
			}
			if (!skillCall)
				curPhase = FRIEND_MOVE;
		}
		break;
		case UnitManager::FRIEND_MOVE:
			//이동의 경우 전원이 동시에 판단


			curPhase = ENEMY_SKILL;
			break;
		case UnitManager::ENEMY_SKILL:
		{
			bool skillCall = false;
			for (auto e : enemies) {
				if (!e->Active())
					continue;
				//행동권 존재
				if (e->GetWait() == 0) {
					//만약 스킬을 쓴다면 
					//시전자의 턴을 증가시키고 
					//리턴
				}
			}
			if (!skillCall)
				curPhase = FRIEND_MOVE;
			curPhase = ENEMY_MOVE;
		}
		break;
		case UnitManager::ENEMY_MOVE:
			//이동의 경우 전원이 동시에 판단
			for (auto e : enemies) {
				auto controller = (EnemyController*)e->GetController();
				controller->SetMoveCommand();
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

void UnitManager::LoadPokemonTable(string fileName)
{
	ifstream ifs;
	ifs.open(fileName);

	if (ifs.fail()) {
		//로드 실패
		return;
	}

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
