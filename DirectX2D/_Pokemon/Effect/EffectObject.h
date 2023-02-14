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
	EffectObject();
	virtual ~EffectObject();

	void Init(
		Unit* unit,
		Effect* effect,
		int applyTarget,
		function<bool(Unit*, Unit*)> condition,
		function<void(Unit*, Unit*)> event,
		POINT startPoint);

	virtual void Update() override;
	virtual void Render() override;

	virtual bool IsCollide() { return false; }

	void Apply();
private:
	Unit* unit = nullptr;					//����Ʈ �߻� ��ü
	int applyTarget = 0;
	//�߻���Ų ��ų�� �ʿ��ұ�? �ڽ����� ��ų ����Ʈ ������Ʈ�� ����� �� ������?
	Effect* effect = nullptr;							//����� ����Ʈ �ִϸ��̼�.  
	function<bool(Unit*, Unit*)> condition = nullptr;	//ȿ�� ���� ���� : Ÿ�� �Ǻ���
	function<void(Unit*, Unit*)> hitEvent = nullptr;	//ȿ�� �Լ�
};