#include "Framework.h"
#include "EffectObject.h"
#include "../Tile/DungeonTileMap.h"
#include "../Unit/UnitManager.h"
#include "../Data/SkillDataManager.h"
#include "../Unit/Unit.h"
#include "../Control/Controller.h"


EffectObject::EffectObject()
	: DungeonObject(Vector2(50.0f, 50.0f))
{
	isActive = false;
}

EffectObject::~EffectObject()
{
	effect = nullptr;
}

void EffectObject::Init(Unit* unit, string effectName, int applyTarget, function<bool(Unit*, Unit*)> condition, function<void(Unit*, Unit*)> event, POINT startPoint)
{
	this->unit = unit;
	this->effect = EffectManager::Get()->Pop(effectName);
	this->condition = condition;
	this->hitEvent = event;
	this->applyTarget = applyTarget;
	isActive = true;

	SetPoint(startPoint);
	if (effect != nullptr)
		effect->Play(GlobalPos());
}

void EffectObject::Update()
{
	//이펙트 적용 조건 : 이펙트 종료 시, 좌표 검사 시
	//이펙트 적용 범위 : 해당 좌표, 공개좌표 전체
	//이펙트 오브젝트가 이펙트 오브젝트를 생성할 수도 있다 - hitEvent에서
	auto target = UnitManager::Get()->GetUnitOnPoint(GetPoint());
	if (!target) {
		//대상 없음
		if (effect) {
			effect->End();
			effect = nullptr;
		}
		isActive = false;
		return;
	}

	if (!effect || !effect->Active()) {
		Apply();
		if (effect)
			effect = nullptr;
		isActive = false;
		return;
	}

	UpdateWorld();
}

void EffectObject::Apply()
{
	//이펙트 종료 시. 기본 공격은 이펙트가 없다
	//발사형이라면 부딪혔을 때여야겠지만.....
	auto target = UnitManager::Get()->GetUnitOnPoint(GetPoint());
	if (!target)	//대상 없음
		return;

	string castTag = unit->GetController()->GetTag();
	string targetTag = target->GetController()->GetTag();
	switch (applyTarget)
	{
	case SkillData::ApplyTarget::ENEMY:
		if (castTag == targetTag ||
			(castTag == "Player" && targetTag == "Friend")
			|| (castTag == "Friend" && targetTag == "Player")) {
			//아군 == 적용 대상 외
			return;
		}
	case SkillData::ApplyTarget::FRIEND:
		if (castTag != targetTag &&
			!((castTag == "Player" && targetTag == "Friend")
			|| (castTag == "Friend" && targetTag == "Player"))) {
			// == 적용 대상 외
			return;
		}
	}

	if (condition(unit, target)) {
		hitEvent(unit, target);
	}
	else {
		//기술 무효 로그
	}

}
