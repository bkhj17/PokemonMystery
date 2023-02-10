#pragma once
class Unit;
class Controller
{
private:
	friend class Unit;

public:
	enum class ControllerType : int {
		UNKNOWN,
		PLAYER,
		FRIEND,
		ENEMY,
	};

	Controller() = default;
	Controller(Unit* unit) : unit(unit) {};
	virtual ~Controller();

	virtual bool SetCommand() = 0;

	void UseSkill(int i);

	string GetTag() { return tag; }
protected:
	void SetUnit(Unit* unit) { this->unit = unit; }

protected:
	Unit* unit = nullptr;
	string tag;
	ControllerType controllerType = ControllerType::UNKNOWN;
};