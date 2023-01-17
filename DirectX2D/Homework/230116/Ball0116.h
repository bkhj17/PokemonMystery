#pragma once

class Ball0116 : public Quad
{
private:
	const float GRAVITY = 500.0f;
	const float BOUND = 0.5f;

public:
	Ball0116(float radius);
	~Ball0116();

	void Update();
	void Render();

	void BarCollision(class Bar0116* bar);
	void AreaCollision(class Area0116* area);

	void BlockCollision(class Block0116* block);

	Vector2& GetVelocity() { return velocity; }

private:
	void ApplyGravity();
	void Move();


private:
	float radius;
	
	CircleCollider* collider;

	Vector2 velocity = {};

};

