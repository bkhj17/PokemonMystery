#pragma once
class Robot0118 : public GameObject
{
	enum ActionType {
		IDLE = 0, RUN,
		JUMP_UP = 10, JUMP_DOWN,
		MELEE = 20, SHOOT,
		JUMP_MELEE, JUMP_SHOOT,
		SPECIAL_SHOOT
	};
public:
	Robot0118();
	~Robot0118();

	void Update();
	void Render();
private:
	void CreateActions();
	void CreateCommands();

	void Control();
	void SpecialShoot();

	void Move();
	void Attack();
	void Jump();

	void InputCommand();
	void SetAction(ActionType type);
private:
	map<ActionType, Action*> actions;
	ActionType curAction = IDLE;
};

