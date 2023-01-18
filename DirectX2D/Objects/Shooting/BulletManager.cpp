#include "Framework.h"
#include "BulletManager.h"

BulletManager::BulletManager()
{
	bullets.resize(30);

	for (auto& bullet : bullets)
		bullet = new Bullet;
}

BulletManager::~BulletManager()
{

	for (auto bullet : bullets)
		delete bullet;
}

void BulletManager::Update()
{
	for (auto bullet : bullets)
		bullet->Update();
}

void BulletManager::Render()
{
	for (auto bullet : bullets)
		bullet->Render();
}

void BulletManager::Fire(Vector2 pos, Vector2 velocity)
{
	for (auto bullet : bullets) {
		if (!bullet->Active()) {
			bullet->Fire(pos, velocity);
			return;
		}
	}
}

void BulletManager::CheckCollision(Collider* collider, Event event)
{
	for (auto bullet : bullets) {
		if (!bullet->Active())
			continue;

		if (bullet->IsCollision(collider)) {
			event();
			bullet->SetActive(false);
		}
	}
}
