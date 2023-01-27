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

	void Fire(Vector2 pos, Vector2 velocity, string tag = "");
	void CheckCollision(Collider* collider, Event event, string tag = "");
private:
	vector<Bullet*> bullets;

	string shooter = "";
};

