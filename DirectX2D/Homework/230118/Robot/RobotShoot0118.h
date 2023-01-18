#pragma once

class RobotShoot0118 : public Action
{
public:
	RobotShoot0118(Transform* target);
	~RobotShoot0118() = default;

	virtual void End();

	void SetFireFunc(Event func) { fireFunc = func; }
protected:
	void Fire();

	virtual void SetFrameEvents();
protected:
	Transform* target;

	Event fireFunc = nullptr;

};

