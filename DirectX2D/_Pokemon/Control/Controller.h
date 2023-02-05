#pragma once
class Unit;
class Controller
{
	friend class Unit;
public:
	Controller();
	Controller(Unit* unit);
	~Controller();

	void SetCommand();
private:
	void SetUnit(Unit* unit) { this->unit = unit; }

private:
	Unit* unit = nullptr;

};

