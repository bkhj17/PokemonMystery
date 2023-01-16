#pragma once
class Block0116 : public Quad
{
public:
	Block0116(Vector2 Size);
	~Block0116();

	void Update();

	void FallDown();

	Collider* GetCollider() { return collider; }

	void Break();

	void SetBreakFunc(function<void()> func) { breakFunc = func; }
private:
	const float FALL_DOWN_RATE = 2.0f;
	float fallDownTime = FALL_DOWN_RATE;

	RectCollider* collider;

	function<void()> breakFunc;
};

