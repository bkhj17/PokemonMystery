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

void BulletManager::Fire(Vector2 pos, Vector2 direction)
{
	for (auto bullet : bullets) {
		if (!bullet->Active()) {
			bullet->Fire(pos, direction);
			return;
		}
	}
}
