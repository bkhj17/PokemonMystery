#pragma once

class Bullet : public Quad
{
public:
	Bullet();
	~Bullet();

	void Update();
	void Render();

	void Fire(Vector2 pos, Vector2 direction);

private:
	float speed = 1000.0f;
	Vector2 direction;
};

