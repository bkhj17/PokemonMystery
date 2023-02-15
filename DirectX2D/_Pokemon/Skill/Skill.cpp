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
	if(data != nullptr)
		curPP = data->maxPP;
}

bool Skill::IsValid()
{
	return data != nullptr;
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

	string effectName = data->name;

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
			EffectManager::Get()->Play(effectName, tileMap->PointToPos(unitPoint) + Vector2(0.0f, 50.0f));
			effectName = "";
			auto tiles = tileMap->DetectableTiles({ unitPoint.x, unitPoint.y });
			for (auto& tilePoint : tiles)
				startPoints.push_back({ tilePoint.first, tilePoint.second });
			break;
		}
	}	

	for (auto& point : startPoints) {
		auto target = UnitManager::Get()->GetUnitOnPoint(point);
		if (target == nullptr)
			continue;
		if (target == unit && data->range != SkillData::SELF)
			continue;
		effectObject->Init(unit, effectName, data->applyTarget,
			bind(&Skill::Condition, this, placeholders::_1, placeholders::_2),
			bind(&Skill::HitEvent, this, placeholders::_1, placeholders::_2),
			point);
	}

	//���� ���� �ÿ��� true ��ȯ
	return true;
}

bool Skill::IsPhysics()
{
	return data->type == SkillData::PHYSICS;
}

bool Skill::IsInRange(POINT castPoint, POINT targetPoint)
{
	//Ÿ������Ʈ�� �νĹ����� ���Դٰ� �����Ѵ�
	//��ȭ��� �����...
	switch (data->range)
	{
	case SkillData::FRONT:
		//���� �����̸� �� �ϰ� �ؾ� ������ �ϴ� ����
	case SkillData::FRONT_THROUGH:
	{
		int distX = abs(targetPoint.x - castPoint.x);
		int distY = abs(targetPoint.y - castPoint.y);
		return max(distX, distY) == 1;
	}
	case SkillData::SELF:
		return true;
	case SkillData::ALL_ROOM:
		return true;
	default:
		break;
	}

	return false;
}

bool Skill::Condition(Unit* cast, Unit* target)
{
	return true;
}

void Skill::HitEvent(Unit* cast, Unit* target)
{
	if (data->type == SkillData::TRANS) {
		target->SetDown(10);
	} else
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

