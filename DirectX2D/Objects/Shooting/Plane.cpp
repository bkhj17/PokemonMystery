#include "Framework.h"

Plane::Plane()
	: Quad(L"Textures/Shooting/airplane.png") 
{
	weapon = new Quad(L"Textures/Shooting/player.png");
	weapon->SetParent(this);
	weapon->Pos().x = 100.0f;

	BulletManager::Get();
}

Plane::~Plane()
{
	delete weapon;

	BulletManager::Delete();
}

void Plane::Update()
{
	Move();
	Rotate();
	Fire();

	__super::Update();
	weapon->Update();
	BulletManager::Get()->Update();
}

void Plane::Render()
{
	Quad::Render();
	weapon->Render();
	BulletManager::Get()->Render();
}

void Plane::Move()
{

	if (KEY_PRESS('W')) {

		localPosition += Right() * speed * DELTA;

		//localPos.y = (localRotation * speed * DELTA);
		//if (localPos.y >= WIN_HEIGHT - Half().y)
		//	localPos.y = WIN_HEIGHT - Half().y;
	}
	if (KEY_PRESS('S')) {

		localPosition -= Right() * speed * DELTA;

		//localPos.y -= speed * DELTA;

		//if (localPos.y <= Half().y)
		//	localPos.y = Half().y;
	}
	/*
	if (KEY_PRESS('A')) {
		localPos.x -= speed * DELTA;

		if (localPos.y <= Half().x)
			localPos.y = Half().x;

	}
	if (KEY_PRESS('D')) {
		localPos.x += speed * DELTA;

		if (localPos.y >= WIN_WIDTH - Half().x)
			localPos.y = WIN_WIDTH - Half().x;
	}
	*/
}

void Plane::Rotate()
{
	//localRotation.z = (mousePos - localPos).Angle();

	if (KEY_PRESS('A')) {
		localRotation.z += DELTA;
	}
	if (KEY_PRESS('D')) {
		localRotation.z -= DELTA;
	}
}

void Plane::Fire()
{
	if (KEY_DOWN(VK_LBUTTON))
		BulletManager::Get()->Fire(weapon->GlobalPos(), Right());
}
