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

void EffectObject::Init(Unit* unit, Effect* effect, int applyTarget, function<bool(Unit*, Unit*)> condition, function<void(Unit*, Unit*)> event, POINT startPoint)
{
	this->unit = unit;
	this->effect = effect;
	this->condition = condition;
	this->hitEvent = event;
	this->applyTarget = applyTarget;
	isActive = true;

	SetPoint(startPoint);
	if (effect != nullptr) {
		Vector2 pos = GlobalPos();
		effect->Play(pos);
	}
}

void EffectObject::Update()
{
	//����Ʈ ���� ���� : ����Ʈ ���� ��, ��ǥ �˻� ��
	//����Ʈ ���� ���� : �ش� ��ǥ, ������ǥ ��ü
	//����Ʈ ������Ʈ�� ����Ʈ ������Ʈ�� ������ ���� �ִ� - hitEvent����
	auto target = UnitManager::Get()->GetUnitOnPoint(GetPoint());
	if (!target) {
		//��� ����
		isActive = false;
		if (effect) {
			effect->SetActive(false);
			effect = nullptr;
		}
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

void EffectObject::Render()
{
	if (!effect || !effect->Active())
		return;
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
