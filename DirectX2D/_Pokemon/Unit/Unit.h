#pragma once
#include "../Object/DungeonObject.h"

class Controller;
struct PokemonData;
struct ItemData;
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

	//bool UseSkill(/*��ų Ű, ���� ��ġ, ���� ���� */); 
	//��ų ���� ������ acting ����? 
	//��ų ��ü�� acting�� �ϰ� ������ ���� �ʳ�?

	Controller* GetController() { return controller; }
	void SetController(Controller* controller);

	int& GetWait() { return wait; }

	void TurnEnd();

	bool PickUpItem(ItemData* itemData);
private:
	void SetAction();
private:
	int animDirX = 0, animDirY = -1;

	AnimObject* animObject;

	//��Ʈ�ѷ� - �÷��̾�, ����, �� �����ؼ� �ൿ ����
	Controller* controller = nullptr;

	int wait = 0; 
	//���� �� 1, ��ÿ� 2, ���� �� 4 ���ҽ�Ű�� 0�̸� �ൿ.... �ٵ� �̷��� ������ ��ϳ�
	//�Ұ����� �������� ���� == �����̻�

	//���ϸ� ����
	PokemonData* data;

	// DungeonObject��(��) ���� ��ӵ�
	virtual bool IsCollide() override;

	vector<class Skill*> skills;

	//���� ������ Ű
	ItemData* carryItem = nullptr;
};

