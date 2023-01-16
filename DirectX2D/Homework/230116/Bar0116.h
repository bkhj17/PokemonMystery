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

	float GetCurAngle() { return curAngle; }

	float GetPower() { return power; }
private:
	bool isLeft;

	bool press = false;
	float speed = 15.0f;
	float up, down;
	float curAngle;

	float power = 100.0f;

	RectCollider* collider;
};

