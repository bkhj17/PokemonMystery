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
		//그 밖에 상태이상도 있다
		//공격할 때 애니메이션 위치만 변경할까?
		//앞으로 나갔다 오는 공격도 결국은 좌표 이동은 없다
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

	//bool UseSkill(/*스킬 키, 시전 위치, 시전 방향 */); 
	//스킬 끝날 때까지 acting 유지? 
	//스킬 객체가 acting을 하고 있으면 되지 않나?

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
	//가속 때 1, 평시에 2, 감속 때 4 감소시키며 0이면 행동.... 근데 이러면 버프는 어떡하냐
	//불가사의 던전에서 버프 == 상태이상
	//
};

