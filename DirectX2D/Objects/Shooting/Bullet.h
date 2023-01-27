#pragma once

class Bullet : public Quad
{
public:
	Bullet();
	~Bullet();

	void Update();
	virtual void UpdateWorld();

	void Render();

	void Fire(Vector2 pos, Vector2 velocity, string tag = "");
	bool IsCollision(Collider* other, string tag = "");

	Collider* GetCollider() { return collider; }
private:
	float speed = 1000.0f;
	Vector2 velocity;

	Collider* collider;

	string shooterTag;
};

