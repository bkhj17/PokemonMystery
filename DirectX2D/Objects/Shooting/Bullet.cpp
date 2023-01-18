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

	localPosition += velocity * speed * DELTA;

	if (localPosition.x < 0 || localPosition.x > WIN_WIDTH)
		isActive = false;


	if (localPosition.y < 0 || localPosition.y > WIN_HEIGHT)
		isActive = false;

	UpdateWorld();
	collider->UpdateWorld();
}

void Bullet::Render()
{
	if (!isActive)
		return;

	__super::Render();
	collider->Render();
}

void Bullet::Fire(Vector2 pos, Vector2 velocity)
{
	isActive = true;

	localPosition = pos;
	this->velocity = velocity;

	localRotation.z = velocity.Angle();
}

bool Bullet::IsCollision(Collider* other)
{ 
	return collider->IsCollision(other);
}
