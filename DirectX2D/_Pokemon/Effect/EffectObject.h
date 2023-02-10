#pragma once
#include "../Object/DungeonObject.h"

class Unit;
class EffectObject : public DungeonObject
{
public:
	//����Ʈ �߻� ��ü
	//����Ʈ �ִϸ��̼�
	//� ȿ�������� �˾ƾ� �Ѵ�
	//���� �´� �� �ؼ� ���� ���� �ʿ��� �Ŵ�
	//�ǰݴ�� �Ǻ����ε� �ʿ��� ���̴�
	EffectObject(
		Unit* unit,
		Effect* effect, 
		function<bool(Unit*, Unit*)> condition, 
		function<void(Unit*, Unit*)> event,
		POINT startPoint
	);
	virtual ~EffectObject();


	virtual void Update() override;
	virtual void Render() override;

	virtual bool IsActing() override;
private:
	Unit* unit;								//����Ʈ �߻� ��ü
	//�߻���Ų ��ų�� �ʿ��ұ�? �ڽ����� ��ų ����Ʈ ������Ʈ�� ����� �� ������?
	//���⼭�� ��ǥ�� �̵��� ���� 
	Effect* effect;							//����� ����Ʈ �ִϸ��̼� : ����Ʈ �Ŵ������� ó��?
	function<bool(Unit*, Unit*)> condition;	//ȿ�� ���� ���� : Ÿ�� �Ǻ���
	function<void(Unit*, Unit*)> hitEvent;	//ȿ�� �Լ�
};