#include "Framework.h"
//#include "Scenes/TutorialScene.h"
//#include "Scenes/SpawnPolygonScene.h"
//#include "Scenes/TextureScene.h"
//#include "Scenes/CollisionScene.h"
//#include "Scenes/PinBallScene.h"
//#include "Scenes/AnimationScene.h"
//#include "Scenes/ShaderScene.h"
//#include "Scenes/PuzzleScene.h"
//#include "Scenes/RenderTargetScene.h"
#include "Scenes/TileScene.h"
#include "Scenes/TankScene.h"
#include "Scenes/DijkstraScene.h"
//#include "Homework/230112/Scene0112.h"
//#include "Homework/230113/Scene0113.h"
//#include "Homework/230116/Scene0116.h"
//#include "Homework/230118/Scene0118.h"
//#include "Homework/230119/Scene0119.h"
//#include "Homework/230120/Scene0120.h"
//#include "Homework/230125/Scene0125.h"
#include "Homework/230126/Scene0126.h"

GameManager::GameManager()
{
	Create();
	uiViewBuffer = new MatrixBuffer;

	scene = new Scene0126();
}

GameManager::~GameManager()
{
	delete scene;
	delete uiViewBuffer;

	Delete();
}

void GameManager::Update()
{
	KeyBoard::Get()->Update();
	Timer::Get()->Update();
	Audio::Get()->Update();

	scene->Update();

	CAM->Update();

	mouseWheel = 0.0f;
}

void GameManager::Render()
{
	scene->PreRender();

	Device::Get()->Clear();

	Font::Get()->GetDC()->BeginDraw();

	Environment::Get()->SetViewport();
	Environment::Get()->SetProjection();

	uiViewBuffer->SetVS(1);
	scene->Render();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
	Font::Get()->RenderText(fps, { 100.0f, WIN_HEIGHT - 20.0f });

	CAM->RenderUI();

	scene->PostRender();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Font::Get()->GetDC()->EndDraw();

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

	Font::Get()->AddColor("White", 1, 1, 1);
	Font::Get()->AddStyle("Default", L"¸¼Àº°íµñ");
	Font::Get()->SetColor("White");
	Font::Get()->SetStyle("Default");

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
	Font::Delete();

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();

	ImGui::DestroyContext();
}
