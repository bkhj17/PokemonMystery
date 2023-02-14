#pragma once

class Unit;
class Skill
{
	//유닛이 써야 할 스킬
public:
	Skill() = default;
	Skill(int i);
	virtual ~Skill() = default;

	void SetData(int i);

	bool Activate(Unit* unit);

	int& GetCurPP() { return curPP; }
protected:
	virtual bool Condition(Unit* cast, Unit* target);
	virtual void HitEvent(Unit* cast, Unit* target);
	
	int CalcDamage(Unit* cast, Unit* target);
protected:
	struct SkillData* data = nullptr;
	int curPP = 0;
};

