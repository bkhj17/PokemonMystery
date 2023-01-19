#include "Framework.h"
//#include "Scenes/TutorialScene.h"
//#include "Scenes/SpawnPolygonScene.h"
//#include "Scenes/TextureScene.h"
//#include "Scenes/CollisionScene.h"
//#include "Scenes/PinBallScene.h"
//#include "Scenes/AnimationScene.h"
#include "Scenes/ShaderScene.h"
//#include "Homework/230112/Scene0112.h"
//#include "Homework/230113/Scene0113.h"
//#include "Homework/230116/Scene0116.h"
//#include "Homework/230118/Scene0118.h"
#include "Homework/230119/Scene0119.h"

GameManager::GameManager()
{
	Create();

	scene = new ShaderScene();
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

	string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
	ImGui::Text(fps.c_str());

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
	Observer::Get();

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
	Observer::Delete();

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();

	ImGui::DestroyContext();
}
