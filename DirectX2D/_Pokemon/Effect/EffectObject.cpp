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
	//����Ʈ ���� ���� : ����Ʈ ���� ��, ��ǥ �˻� ��
	//����Ʈ ���� ���� : �ش� ��ǥ, ������ǥ ��ü
	//����Ʈ ������Ʈ�� ����Ʈ ������Ʈ�� ������ ���� �ִ� - hitEvent����
	auto target = UnitManager::Get()->GetUnitOnPoint(GetPoint());
	if (!target) {
		//��� ����
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
	//����Ʈ ���� ��. �⺻ ������ ����Ʈ�� ����
	//�߻����̶�� �ε����� �����߰�����.....
	auto target = UnitManager::Get()->GetUnitOnPoint(GetPoint());
	if (!target)	//��� ����
		return;

	string castTag = unit->GetController()->GetTag();
	string targetTag = target->GetController()->GetTag();
	switch (applyTarget)
	{
	case SkillData::ApplyTarget::ENEMY:
		if (castTag == targetTag ||
			(castTag == "Player" && targetTag == "Friend")
			|| (castTag == "Friend" && targetTag == "Player")) {
			//�Ʊ� == ���� ��� ��
			return;
		}
	case SkillData::ApplyTarget::FRIEND:
		if (castTag != targetTag &&
			!((castTag == "Player" && targetTag == "Friend")
			|| (castTag == "Friend" && targetTag == "Player"))) {
			// == ���� ��� ��
			return;
		}
	}

	if (condition(unit, target)) {
		hitEvent(unit, target);
	}
	else {
		//��� ��ȿ �α�
	}

}
