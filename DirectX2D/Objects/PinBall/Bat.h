#pragma once

class Bat : public Quad
{
private:
	const float LEFT_MIN_ANGLE = XMConvertToRadians(-30.0f);
	const float LEFT_MAX_ANGLE = XMConvertToRadians(30.0f);
	const float RIGHT_MIN_ANGLE = XMConvertToRadians(210.0f);
	const float RIGHT_MAX_ANGLE = XMConvertToRadians(150.0f);

public:
	Bat(bool isRight);
	~Bat();

	void Update();
	void Render();

	void Control();

	Collider* GetCollider() { return collider; }

	bool IsRight() { return isRight; }
	bool IsSwing() { return isSwing; }
	float GetInterpolationValue() {	return interpolationValue; }

private:
	bool isRight;
	bool isSwing = false;

	float speed = 10.0f;
	float interpolationValue = 0.0f;

	Collider* collider;
};

