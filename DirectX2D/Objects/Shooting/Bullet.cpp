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
}

void Bullet::UpdateWorld()
{
	__super::UpdateWorld();
	collider->UpdateWorld();
}

void Bullet::Render()
{
	if (!isActive)
		return;

	__super::Render();
	collider->Render();
}

void Bullet::Fire(Vector2 pos, Vector2 velocity, string tag)
{
	isActive = true;

	shooterTag = tag;

	localPosition = pos;
	this->velocity = velocity;

	localRotation.z = velocity.Angle();
	UpdateWorld();
}

bool Bullet::IsCollision(Collider* other, string tag)
{ 
	if (shooterTag == tag)
		return false;

	Vector2 bpos = GlobalPos();
	Vector2 cpos = other->GlobalPos();
	return collider->IsCollision(other);
}
