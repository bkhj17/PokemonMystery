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

	virtual void Init() {}
	virtual bool SetCommand() = 0;

	string GetTag() { return tag; }

	virtual void TurnEnd() {}
protected:
	void SetUnit(Unit* unit) { this->unit = unit; }

protected:
	Unit* unit = nullptr;
	string tag;
	ControllerType controllerType = ControllerType::UNKNOWN;
};