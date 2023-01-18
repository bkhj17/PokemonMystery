#pragma once
class NinjaJump : public Action
{
private:
	const float JUMP_POWER = 500.0f;
public:
	NinjaJump(Transform* target);
	~NinjaJump() = default;

	virtual void Update() override;
	virtual void Start() override;

private:
	Transform* target;
	float velocity = 0.0f;
};

