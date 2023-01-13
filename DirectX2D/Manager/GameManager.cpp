#include "Framework.h"
//#include "Scenes/TutorialScene.h"
//#include "Scenes/SpawnPolygonScene.h"
//#include "Homework/230112/Scene0112.h"
#include "Homework/230113/Scene0113.h"
//#include "Scenes/TextureScene.h"
GameManager::GameManager()
{
	Create();

	scene = new Scene0113();
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
	Audio::Get()->Update();

	scene->Update();
}

void GameManager::Render()
{
	Device::Get()->Clear();



	scene->Render();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	scene->PostRender();


	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Device::Get()->Present();
}

void GameManager::Create()
{
	KeyBoard::Get();
	Timer::Get();
	Device::Get();
	Environment::Get();
	Audio::Get();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete()
{
	KeyBoard::Delete();
	Timer::Delete();
	Device::Delete();
	Shader::Delete();
	Texture::Delete();
	Environment::Delete();
	Audio::Delete();

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();

	ImGui::DestroyContext();
}
