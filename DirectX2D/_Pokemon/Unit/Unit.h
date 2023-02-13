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

	//bool UseSkill(/*스킬 키, 시전 위치, 시전 방향 */); 
	//

	Controller* GetController() { return controller; }
	void SetController(Controller* controller);

	int& GetWait() { return wait; }

	void TurnEnd();

	bool PickUpItem(ItemData* itemData);
private:
	void SetAction();

	//물리 공격 시 animObject가 앞으로 갔다와야 한다

	// DungeonObject을(를) 통해 상속됨
	virtual bool IsCollide() override;

	// DungeonObject을(를) 통해 상속됨
	virtual bool UseSkill(int index);
	void SetIdle();

	void SkillActivate();

private:
	int animDirX = 0, animDirY = -1;
	
	AnimObject* animObject;
	//갔다 온다....

	//컨트롤러 - 플레이어, 동료, 적 구분해서 행동 결정
	Controller* controller = nullptr;

	int wait = 0; 
	//가속 때 1, 평시에 2, 감속 때 4 감소시키며 0이면 행동.... 근데 이러면 버프는 어떡하냐
	//불가사의 던전에서 버프 == 상태이상

	//포켓몬 정보
	PokemonData* data;

	vector<class Skill*> skills;

	//지닌 아이템 키
	ItemData* carryItem = nullptr;

	//
	int curSkill = -1;

	//현재의 행동 코드
	int dirCode = 0;

};

