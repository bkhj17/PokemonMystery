#pragma once
class Bullet0112 : public GameObject
{
private:
	const float RADIUS = 10.0f;
	const float SPEED = 1000.0f;
public:
	Bullet0112();
	~Bullet0112();

	void Update();
	void Render();

	void Spawn(Vector2 pos, float angle);

	bool IsActive() { return isActive; }
private:
	bool isActive = false;

	class Transform0112* transform;
};

