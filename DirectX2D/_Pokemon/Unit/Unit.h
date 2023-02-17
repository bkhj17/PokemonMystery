#pragma once
#include "../Object/DungeonObject.h"

class Controller;
struct PokemonData;
struct ItemData;
class Unit : public DungeonObject
{
private:
	friend class Controller;
	friend class PlayerController;
	friend class EnemyController;
	friend class UnitManager;

	const float PHYSICS_TIME = 0.3f;
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

	void Init();
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
	
	Controller* GetController() { return controller; }
	void SetController(Controller* controller);

	int& GetWait() { return wait; }

	void TurnEnd();

	bool PickUpItem(ItemData* itemData);

	// DungeonObject��(��) ���� ��ӵ�
	virtual bool IsCollide() override;

	// DungeonObject��(��) ���� ��ӵ�
	vector<class Skill*>& GetSkills() { return skills; };
	virtual bool IsUsableSkill(int index, bool log = true);
	virtual bool UseSkill(int index);

	void Damage(int damage, bool log = true);
	void Die();

	int GetCurAction() { return dirCode % 100; }

	void SetDown(int d);
private:
	void SetAction();

	//���� ���� �� animObject�� ������ ���ٿ;� �Ѵ�

	void SetIdle();

	void SkillActivate();

	void InitSkillData(int level);
	void SetSkill(int slot, int skillIndex);
	void AddNewSkill(int level);

private:
	//���ϸ� ����
	PokemonData* data;

	//animObject ����
	int animDirX = 0, animDirY = -1;
	//������ �ൿ �ڵ�
	int dirCode = 0;
	AnimObject* animObject;
	Vector2 animOffset = {};
	float animTime = 0.0f;
	float animRate = 0.3f;

	//���� �´�....

	//��Ʈ�ѷ� - �÷��̾�, ����, �� �����ؼ� �ൿ ����
	Controller* controller = nullptr;

	int wait = 0; 
	//���� �� 1, ��ÿ� 2, ���� �� 4 ���ҽ�Ű�� 0�̸� �ൿ.... �ٵ� �̷��� ������ ��ϳ�
	//�Ұ����� �������� ���� == �����̻�

	int curSkill = -1;
	vector<class Skill*> skills;

	//���� ������ Ű
	ItemData* carryItem = nullptr;

	int downTime = 0;
	Quad* downQuad = nullptr;

};

