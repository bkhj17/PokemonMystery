#include "Framework.h"
#include "GameManager.h"
#include "Scenes/TutorialScene.h"

GameManager::GameManager()
{
	Create();

	scene = new TutorialScene;
}

GameManager::~GameManager()
{
	delete scene;

	Delete();
}

void GameManager::Update()
{
	KeyBoard::Get()->Update();
	Timer::Get()->Update();

	scene->Update();
}

void GameManager::Render()
{
	Device::Get()->Clear();

	scene->Render();
	scene->PostRender();

	Device::Get()->Present();
}

void GameManager::Create()
{
	KeyBoard::Get();
	Timer::Get();
	Device::Get();
}

void GameManager::Delete()
{
	KeyBoard::Delete();
	Timer::Delete();
	Device::Delete();
	Shader::Delete();
}
