#pragma once

class Ball0116 : public Quad
{
public:
	Ball0116(float radius);
	~Ball0116();

	void Update();
	void Render();

	void BarCollision(class Bar0116* bar);
	void AreaCollision(class Area0116* area);

	void BlockCollision(class Block0116* block);

	Vector2& GetDirection() { return direction; }

private:
	void ApplyGravity();
	void Move();


private:
	float radius;
	
	CircleCollider* collider;

	Vector2 direction = {};
	
	float speedLimit = 600.0f;

};

