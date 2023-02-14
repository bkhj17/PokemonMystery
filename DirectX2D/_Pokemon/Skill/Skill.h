#pragma once

class Unit;
class Skill
{
	//������ ��� �� ��ų
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

