#pragma once

class Ball : public Quad
{
private:
	const float GRAVITY = 500.0f;
	const float BOUND = 0.5f;

public:
	Ball();
	~Ball();

	void Update();
	void Render();

	CircleCollider* GetCollider() { return collider; }

	void CollisionBat(Bat* bat);
private:
	void CollisionBorder();

private:
	Vector2 velocity;

	CircleCollider* collider;
};

