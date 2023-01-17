#pragma once

class Bar0116 : public Quad
{
public:
	Bar0116(Vector2 size, bool isLeft);
	~Bar0116();

	void Update();
	void Render();

	RectCollider* GetCollider() { return collider; }

	void SetPress(bool press) { this->press = press; }

	float GetPower() { return power; }

	float GetLength() { return size.x * localScale.x; }

	void Control();

	bool IsLeft() { return isLeft; }
	bool IsSwing() { return isSwing; }
	float GetInterpolation() { return interpolationValue; }

private:
	bool isLeft;

	bool press = false;
	float speed = 15.0f;
	float up, down;

	float power = 100.0f;


	bool isSwing = false;
	float interpolationValue = 0.0f;

	RectCollider* collider;
};

