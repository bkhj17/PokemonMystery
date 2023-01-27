#include "Framework.h"
#include "BulletManager.h"

BulletManager::BulletManager()
{
	bullets.resize(30);

	for (auto& bullet : bullets) {
		bullet = new Bullet;
		bullet->Scale() *= 0.5f;
		bullet->GetCollider()->Scale() *= { 0.9f, 0.6f};
	}
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


void BulletManager::Fire(Vector2 pos, Vector2 velocity, string tag)
{
	for (auto bullet : bullets) {
		if (!bullet->Active()) {
			bullet->Fire(pos, velocity, tag);
			return;
		}
	}
}

void BulletManager::CheckCollision(Collider* collider, Event event, string tag)
{
	if (!collider->Active())
		return;

	for (auto bullet : bullets) {
		if (!bullet->Active())
			continue;

		if (bullet->IsCollision(collider, tag)) {
			if(event != nullptr)
				event();
			bullet->SetActive(false);
		}
	}
}
