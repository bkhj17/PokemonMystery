#include "Framework.h"
#include "Bullet.h"

Bullet::Bullet()
	: Quad(L"Textures/Shooting/playerBullet.png")
{
	isActive = false;

	collider = new RectCollider(texture->GetSize());
	collider->SetParent(this);
}

Bullet::~Bullet()
{
	delete collider;
}

void Bullet::Update()
{
	if (!isActive)
		return;

	localPosition += direction * speed * DELTA;

	if (localPosition.x < 0 || localPosition.x > WIN_WIDTH)
		isActive = false;


	if (localPosition.y < 0 || localPosition.y > WIN_HEIGHT)
		isActive = false;

	__super::Update();
	collider->UpdateWorld();
}

void Bullet::Render()
{
	if (!isActive)
		return;

	__super::Render();
	collider->Render();
}

void Bullet::Fire(Vector2 pos, Vector2 direction)
{
	isActive = true;

	localPosition = pos;
	this->direction = direction;

	localRotation.z = direction.Angle();
}
