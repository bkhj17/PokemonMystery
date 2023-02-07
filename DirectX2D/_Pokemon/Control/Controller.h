#pragma once
class Unit;
class Controller
{
	friend class Unit;
public:
	Controller() = default;
	Controller(Unit* unit) : unit(unit) {};
	virtual ~Controller() = default;

	virtual void SetCommand() = 0;
protected:
	void SetUnit(Unit* unit) { this->unit = unit; }

protected:
	Unit* unit = nullptr;
	string tag;
};

