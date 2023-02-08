#pragma once
class Unit;
class Controller
{
	friend class Unit;
public:
	Controller() = default;
	Controller(Unit* unit) : unit(unit) {};
	virtual ~Controller() = default;

	virtual bool SetCommand() = 0;

	void UseSkill(int i);

	string GetTag() { return tag; }
protected:
	void SetUnit(Unit* unit) { this->unit = unit; }

protected:
	Unit* unit = nullptr;
	string tag;
};

