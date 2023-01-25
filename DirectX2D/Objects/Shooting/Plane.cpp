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

	UpdateWorld();
	weapon->UpdateWorld();
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
	if (KEY_PRESS(upKey)) {
		localPosition += Right() * speed * DELTA;
	}
	if (KEY_PRESS(downKey)) {
		localPosition -= Right() * speed * DELTA;
	}
}

void Plane::Rotate()
{
	//localRotation.z = (mousePos - localPos).Angle();

	if (KEY_PRESS(leftKey)) {
		localRotation.z += turnSpeed * DELTA;
	}
	if (KEY_PRESS(rightKey)) {
		localRotation.z -= turnSpeed * DELTA;
	}
}

void Plane::Fire()
{
	if (KEY_DOWN(VK_LBUTTON))
		BulletManager::Get()->Fire(weapon->GlobalPos(), Right());
}
