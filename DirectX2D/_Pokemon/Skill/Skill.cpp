#include "Framework.h"
#include "Skill.h"
#include "../Data/SkillDataManager.h"
#include "../Effect/EffectObject.h"
#include "../Effect/EffectObjectManager.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitManager.h"
#include "../Tile/DungeonTileMap.h"

Skill::Skill(int i)
{
	SetData(i);
}

void Skill::SetData(int i)
{
	//i�� ��ų �Ŵ������� ������ �޾ƿ���
	data = SkillDataManager::Get()->GetData(i);
	curPP = data->maxPP;
}

bool Skill::Activate(Unit* unit)
{
	if (!data)
		return false;

	auto effectObject = EffectObjectManager::Get()->Pop();
	//�ٷ� ��
	vector<POINT> startPoints;

	POINT unitPoint = unit->GetPoint();

	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);

	switch (data->range)
	{
		case SkillData::FRONT:
		{
			startPoints.push_back({ unitPoint.x + unit->GetDirX(), unitPoint.y + unit->GetDirY() });
			break;
		}
		case SkillData::FRONT_THROUGH:
		{
			startPoints.push_back({ unitPoint.x + unit->GetDirX(), unitPoint.y + unit->GetDirY() });
			break;
		}
		case SkillData::SHOT:
		{
			break;
		}
		case SkillData::ALL_ROOM:
		{
			auto tiles = tileMap->DetectableTiles({ unitPoint.x, unitPoint.y });
			for (auto& tilePoint : tiles)
				startPoints.push_back({ tilePoint.first, tilePoint.second });
			break;
		}
	}

	for (auto& point : startPoints)
		effectObject->Init(unit, EffectManager::Get()->Pop("����ä��"), data->applyTarget,
			bind(&Skill::Condition, this, placeholders::_1, placeholders::_2), 
			bind(&Skill::HitEvent, this, placeholders::_1, placeholders::_2), 
			point);

	//���� ���� �ÿ��� true ��ȯ
	return true;
}

bool Skill::Condition(Unit* cast, Unit* target)
{
	return true;
}

void Skill::HitEvent(Unit* cast, Unit* target)
{
	target->Damage(CalcDamage(cast, target));
}

int Skill::CalcDamage(Unit* cast, Unit* target)
{
	const auto& castData = cast->GetData()->statusData;
	const auto& targetData = target->GetData()->statusData;

	int power = data->power;
	int levelValue = cast->GetData()->level * 2 / 5 + 2;
	int attack = data->type == SkillData::PHYSICS ? castData.attack : castData.sAttack;
	int defence = data->type == SkillData::PHYSICS ? targetData.defense : targetData.sDefense;
	float critical = Random(0, 24) == 0 ? 1.5f : 1.0f;
	float random = Random(85, 100) * 0.01f;
	float specialty = find(castData.type.begin(), castData.type.end(), data->pokemonType) == castData.type.end() ? 1.0f : 1.5f;
	float advantage = SkillDataManager::Get()->CalcAdvantage(data->pokemonType, targetData.type[0]);
	if(targetData.type.size() == 2)
		advantage *=  SkillDataManager::Get()->CalcAdvantage(data->pokemonType, targetData.type[1]);

	int damage = static_cast<int>(floorf((((levelValue * power * attack / 50) / defence) + 2) * critical * random) * specialty * advantage);

	return damage;
}

