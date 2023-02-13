#include "Framework.h"
#include "EffectObject.h"
#include "../Tile/DungeonTileMap.h"
#include "../Unit/UnitManager.h"

EffectObject::EffectObject()
	: DungeonObject(Vector2(50.0f, 50.0f))
{
}

EffectObject::~EffectObject()
{
	delete effect;
}

void EffectObject::Init(Unit* unit, Effect* effect, function<bool(Unit*, Unit*)> condition, function<void(Unit*, Unit*)> event, POINT startPoint)
{
	this->unit = unit;
	this->effect = effect;
	this->condition = condition;
	this->hitEvent = event;

	SetPoint(startPoint);
	effect->SetParent(this);
}

void EffectObject::Update()
{
	//이펙트 적용 조건 : 이펙트 종료 시, 좌표 검사 시
	//이펙트 적용 범위 : 해당 좌표, 공개좌표 전체
	//이펙트 오브젝트가 이펙트 오브젝트를 생성할 수도 있다 - hitEvent에서	
	if (!effect->Active())
		return;

	if(effect)
		effect->Update();

	if (!effect || !effect->Active()) {
		//이펙트 종료 시
		isActive = false;
		
		auto target = UnitManager::Get()->GetUnitOnPoint(GetPoint());		
		if (condition(unit, target)) {
			hitEvent(unit, target);
		} else {
			//기술 무효 로그
		}

		return;
	}

	UpdateWorld();
}

void EffectObject::Render()
{
	if (!effect)
		return;

	if (!effect->Active())
		return;

	effect->Render();
}