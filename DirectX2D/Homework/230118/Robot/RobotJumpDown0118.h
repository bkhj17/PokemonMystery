#pragma once

class RobotJumpDown0118 : public Action
{
public:
	RobotJumpDown0118(Transform* target);
	~RobotJumpDown0118() = default;

	void Update();
	virtual void Start() override;
private:
	Transform* target;
	float velocity = 0.0f;
};

