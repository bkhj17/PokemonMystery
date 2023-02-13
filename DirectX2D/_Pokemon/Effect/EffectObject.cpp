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
	//����Ʈ ���� ���� : ����Ʈ ���� ��, ��ǥ �˻� ��
	//����Ʈ ���� ���� : �ش� ��ǥ, ������ǥ ��ü
	//����Ʈ ������Ʈ�� ����Ʈ ������Ʈ�� ������ ���� �ִ� - hitEvent����	
	if (!effect->Active())
		return;

	if(effect)
		effect->Update();

	if (!effect || !effect->Active()) {
		//����Ʈ ���� ��
		isActive = false;
		
		auto target = UnitManager::Get()->GetUnitOnPoint(GetPoint());		
		if (condition(unit, target)) {
			hitEvent(unit, target);
		} else {
			//��� ��ȿ �α�
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