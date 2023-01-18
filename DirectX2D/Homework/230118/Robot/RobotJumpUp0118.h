#pragma once

class RobotJumpUp0118 : public Action
{
private:
	const float JUMP_POWER = 500.0f;
public:
	RobotJumpUp0118(Transform* target);
	~RobotJumpUp0118() = default;

	void Update();
	virtual void Start() override;
private:
	Transform* target;
	float velocity = 0.0f;
};

