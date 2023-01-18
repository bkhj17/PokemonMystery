#include "Framework.h"
#include "Scene0118.h"
#include "Command0118.h"
#include "Robot/Robot0118.h"
#include "Dummy0118.h"

Scene0118::Scene0118()
{
	Command0118::Get();
	BulletManager::Get();

	robot = new Robot0118();
	Dummy0118::Get()->Pos() = { CENTER_X + 200.0f, 200.0f };
}

Scene0118::~Scene0118()
{
	Command0118::Delete();
	BulletManager::Delete();
	Dummy0118::Delete();
	delete robot;
}

void Scene0118::Update()
{
	robot->Update();
	Dummy0118::Get()->Update();
	BulletManager::Get()->Update();

	BulletManager::Get()->CheckCollision(Dummy0118::Get()->GetCollider(), bind(&Dummy0118::Hit, Dummy0118::Get()));
}

void Scene0118::Render()
{
	robot->Render();
	BulletManager::Get()->Render();
	Dummy0118::Get()->Render();

}

void Scene0118::PostRender()
{
	Command0118::Get()->PostRender();
}
