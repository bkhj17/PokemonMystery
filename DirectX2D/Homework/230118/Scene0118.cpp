#include "Framework.h"
#include "Scene0118.h"
#include "Command0118.h"
#include "Robot/Robot0118.h"

Scene0118::Scene0118()
{
	Command0118::Get();
	BulletManager::Get();
	robot = new Robot0118();

}

Scene0118::~Scene0118()
{
	Command0118::Delete();
	BulletManager::Delete();
	//Dummy0118::Delete();
	delete robot;
}

void Scene0118::Update()
{
	robot->Update();
	BulletManager::Get()->Update();
}

void Scene0118::Render()
{
	robot->Render();
	BulletManager::Get()->Render();
}

void Scene0118::PostRender()
{
	Command0118::Get()->PostRender();
}
