#pragma once
#include "../Object/DungeonObject.h"

class Controller;
class Unit : public DungeonObject
{
private:
	friend class Controller;

	enum ActionState {
		IDLE,
		MOVING,
		USE_SKILL_STANDING,
		USE_SKILL_FORWARD,
		DAMAGE,
		//�� �ۿ� �����̻� �ִ�
		//������ �� �ִϸ��̼� ��ġ�� �����ұ�?
		//������ ������ ���� ���ݵ� �ᱹ�� ��ǥ �̵��� ����
	};


public:
	Unit(Vector2 size);
	Unit(Controller* controller, Vector2 size);
	virtual ~Unit();

	void Update();
	void Render();

	void CreateClipData();
	void UpdateWorld();

	void SetDir(int x, int y);

	bool IsActing();

	//bool UseSkill(/*��ų Ű, ���� ��ġ, ���� ���� */); 
	//��ų ���� ������ acting ����? 
	//��ų ��ü�� acting�� �ϰ� ������ ���� �ʳ�?

	//Damage(); void? int?

	Controller* GetController() { return controller; }
	void SetController(Controller* controller);

	int& GetWait() { return wait; }

	void TurnEnd();

private:
	void SetAction();
private:
	int animDirX = 0, animDirY = -1;

	AnimObject* animObject;

	Controller* controller = nullptr;

	int wait = 0; 
	//���� �� 1, ��ÿ� 2, ���� �� 4 ���ҽ�Ű�� 0�̸� �ൿ.... �ٵ� �̷��� ������ ��ϳ�
	//�Ұ����� �������� ���� == �����̻�
	//
};

