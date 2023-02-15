#pragma once

class Unit;
struct SkillData;
class Skill
{
	//유닛이 써야 할 스킬
public:
	Skill() = default;
	Skill(int i);
	virtual ~Skill() = default;

	void SetData(int i);
	bool IsValid();
	bool Activate(Unit* unit);
	int& GetCurPP() { return curPP; }

	SkillData* GetData() { return data; }

	bool IsPhysics();

	//사거리에 들어오는지 확인
	bool IsInRange(POINT castPoint, POINT targetPoint);
protected:
	virtual bool Condition(Unit* cast, Unit* target);
	virtual void HitEvent(Unit* cast, Unit* target);
	
	int CalcDamage(Unit* cast, Unit* target);
protected:
	SkillData* data = nullptr;
	int curPP = 0;
};