#include "Framework.h"
#include "EffectObject.h"
#include "../Tile/DungeonTileMap.h"

EffectObject::EffectObject(Unit* unit, Effect* effect, function<bool(Unit*, Unit*)> condition, function<void(Unit*, Unit*)> event, POINT startPoint)
	: DungeonObject(Vector2(50.0f, 50.0f)), unit(unit), effect(effect), condition(condition), hitEvent(event)
{
	SetPoint(startPoint);
	effect->SetParent(this);	//����Ʈ ���� �ִԿ�����Ʈ�� ��� �ϳ�?
	//������ ���� �־ �����ϴ�
	 
	
	//����Ʈ�� ��������� �߰��Ѵ�? 
	// 
	// 
	// 
	//����� ������ ����Ʈ�� �� ���̱⿡ ������Ʈ ũ��� ���ʿ�������?
	//Ÿ�ϸ��� �̱������� �ٲ������...
	//������Ʈ�� �Ǵµ�...
	//���� ������Ʈ�� ������� �ݶ��̴� ũ�⸦ ���ϴ� ���̴ٸ� �ݶ��̴� ��ü�� �� ���� �ִ�
}

EffectObject::~EffectObject()
{
	delete effect;
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