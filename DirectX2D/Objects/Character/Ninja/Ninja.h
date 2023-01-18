#pragma once

class Clip;

class Ninja : public GameObject
{
private:
	enum ActionType {
		IDLE = 0, RUN, JUMP,
		ATTACK = 10, 
		JUMP_ATTACK, THROW,	JUMP_THROW,
		DEAD, GLIDE, SLIDE,
	};

public:
	Ninja();
	~Ninja();

	void Update();
	void Render();

private:
	void CreateActions();

	void Control();
	void Move();
	void Attack();
	void Jump();

	void SetIdle() { SetAction(IDLE); }
	void Land();
	void EndAttack();

	void SetAction(ActionType type);

private:
	map<ActionType, Action*> actions;
	ActionType curAction = IDLE;
};

