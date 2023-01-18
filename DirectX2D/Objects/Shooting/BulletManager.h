#pragma once
class BulletManager : public Singleton<BulletManager>
{
private:
	friend class Singleton;
	BulletManager();
	~BulletManager();

public:
	void Update();
	void Render();

	void Fire(Vector2 pos, Vector2 velocity);
	void CheckCollision(Collider* collider, Event event);
private:
	vector<Bullet*> bullets;
};

