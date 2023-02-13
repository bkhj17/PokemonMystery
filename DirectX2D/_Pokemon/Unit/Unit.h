#pragma once
#include "../Object/DungeonObject.h"

class Controller;
struct PokemonData;
struct ItemData;
class Unit : public DungeonObject
{
private:
	friend class Controller;

public:
	enum ActionState {
		IDLE,
		MOVING,
		SKILL_PHYSICS,
		SKILL_SPECIAL,
		DAMAGE,
	};

	Unit(Controller* controller, Vector2 size);
	virtual ~Unit();

	void Update();
	void Render();

	void CreateClipData();
	void UpdateWorld();

	void SetDir(int x, int y);
	int GetDirX() { return animDirX; };
	int GetDirY() { return animDirY; };

	void SetData(int key, int level);
	PokemonData* GetData() { return data; }
	void SetLevelData(int level);

	bool IsActing();

	bool Test();

	//bool UseSkill(/*��ų Ű, ���� ��ġ, ���� ���� */); 
	//

	Controller* GetController() { return controller; }
	void SetController(Controller* controller);

	int& GetWait() { return wait; }

	void TurnEnd();

	bool PickUpItem(ItemData* itemData);
private:
	void SetAction();

	//���� ���� �� animObject�� ������ ���ٿ;� �Ѵ�

	// DungeonObject��(��) ���� ��ӵ�
	virtual bool IsCollide() override;

	// DungeonObject��(��) ���� ��ӵ�
	virtual bool UseSkill(int index);
	void SetIdle();

	void SkillActivate();

private:
	int animDirX = 0, animDirY = -1;
	
	AnimObject* animObject;
	//���� �´�....

	//��Ʈ�ѷ� - �÷��̾�, ����, �� �����ؼ� �ൿ ����
	Controller* controller = nullptr;

	int wait = 0; 
	//���� �� 1, ��ÿ� 2, ���� �� 4 ���ҽ�Ű�� 0�̸� �ൿ.... �ٵ� �̷��� ������ ��ϳ�
	//�Ұ����� �������� ���� == �����̻�

	//���ϸ� ����
	PokemonData* data;

	vector<class Skill*> skills;

	//���� ������ Ű
	ItemData* carryItem = nullptr;

	//
	int curSkill = -1;

	//������ �ൿ �ڵ�
	int dirCode = 0;

};

