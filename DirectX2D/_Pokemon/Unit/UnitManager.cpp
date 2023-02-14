#include "Framework.h"
#include "UnitManager.h"
#include "Unit.h"
#include "../Control/PlayerController.h"
#include "../Control/EnemyController.h"
#include "../Tile/DungeonTileMap.h"

UnitManager::UnitManager()
{
	LoadPokemonTable("TextData/PokemonData.csv");
	LoadAnimData();

	player = new Unit(new PlayerController(), UNIT_SIZE);

	friends.reserve(3);
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

	Unit* testEnemy = new Unit(new EnemyController(), UNIT_SIZE);
	testEnemy->SetData(19, 2);

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
				if (player->GetController()->SetCommand())
					player->GetWait() += 2;
				else
					return;
			}
			curPhase = PLAYER_ACT;
			break;
		case UnitManager::PLAYER_ACT:
			curPhase = FRIEND_SKILL;
			//�̵� �ܿ� �ٸ��� �ϴ� ���̸� ����
			break;
		case UnitManager::FRIEND_SKILL:
		{
			bool skillCall = false;
			for (auto f : friends) {
				if (!f->Active())
					continue;
				//�ൿ�� ����
				if (f->GetWait() == 0) {
					//���� ��ų�� ���ٸ�
					if (false) {
						//�������� ���� ������Ű��
						//����
						f->GetWait() += 2;
						return;
					}
				}
			}
			if (!skillCall)
				curPhase = FRIEND_MOVE;
		}
		break;
		case UnitManager::FRIEND_MOVE:
			//�̵��� ��� ������ ���ÿ� �Ǵ�
			curPhase = ENEMY_SKILL;

			for (auto f : friends) {

			}
			break;
		case UnitManager::ENEMY_SKILL:
		{
			bool skillCall = false;
			for (auto e : enemies) {
				if (!e->Active())
					continue;
				//�ൿ�� ����
				if (e->GetWait() == 0) {
					//���� ��ų�� ���ٸ� 
					//�������� ���� ������Ű�� 
					//����
				}
			}
			if (!skillCall)
				curPhase = FRIEND_MOVE;
			curPhase = ENEMY_MOVE;
		}
		break;
		case UnitManager::ENEMY_MOVE:
			//�̵��� ��� ������ ���ÿ� �Ǵ�
			for (auto e : enemies) {
				auto controller = (EnemyController*)e->GetController();
				controller->SetMoveCommand();
			}

			curPhase = TURN_END;
			return;
		case UnitManager::TURN_END:
			//��� ������ �� ����
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
		if (f->Active() && f->IsActing())
			return true;
	for (auto e : enemies)
		if (e->Active() && e->IsActing())
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

bool UnitManager::CheckMovablePoint(POINT point, int dirX, int dirY)
{
	//�� ���� �ҷ��� �� �ִ��� Ȯ��
	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);
	if (tileMap == nullptr)
		return false;

	Vector2 destPos = {};	//�Լ� ȣ��� ����
	//�� �� �ִ� ��ġ���� Ȯ��
	if (!tileMap->SetMove(point.x, point.y, dirX, dirY, destPos))
		return false;

	POINT nextPos = point;
	nextPos.x += dirX;
	nextPos.y += dirY;
	//���� �� �ڸ��� ���� ���� �ڸ���� �ִ����� �˾ƾ� ��
	if (IsUnitOnPoint(nextPos))
		return false;

	return true;
}

void UnitManager::ApplyLevel(IN int level, OUT PokemonData* data)
{
	//�⺻ ������ ������ ����
	PokemonTableData tableData = pokemonTable[data->key];
	//���� �ݿ�
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
		//�ε� ����
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

void UnitManager::LoadAnimData()
{	
	AnimData data;
	data.pokemonNum = 1;
	data.textureFile = L"Textures/pokemon/�̻��ؾ�.png";
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

		//4�� �߾� => ��꿡 ���� �ʾ� ���� �ʴ´�

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
	data.textureFile = L"Textures/pokemon/����.png";
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

		//4�� �߾� => ��꿡 ���� �ʾ� ���� �ʴ´�

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
