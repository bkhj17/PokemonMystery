#pragma once

class Unit;
struct SkillData;
class Skill
{
	//������ ��� �� ��ų
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

	//��Ÿ��� �������� Ȯ��
	bool IsInRange(POINT castPoint, POINT targetPoint);
protected:
	virtual bool Condition(Unit* cast, Unit* target);
	virtual void HitEvent(Unit* cast, Unit* target);
	
	int CalcDamage(Unit* cast, Unit* target);
protected:
	SkillData* data = nullptr;
	int curPP = 0;
};