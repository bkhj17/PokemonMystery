#include "Framework.h"
#include "Scenes/TileScene.h"

#include "_Pokemon/Scene/DungeonScene.h"
#include "_Pokemon/Scene/ClearScene.h"
GameManager::GameManager()
{
	Create();
	uiViewBuffer = new MatrixBuffer;

	SceneManager::Get()->Add("Dungeon", new DungeonScene());
	SceneManager::Get()->Add("Clear", new ClearScene());
	SceneManager::Get()->ChangeScene("Dungeon");
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

	SceneManager::Get()->Update();

	CAM->Update();

	mouseWheel = 0.0f;

	if (KEY_UP(VK_F1))
		showFPS = !showFPS;
}

void GameManager::Render()
{
	SceneManager::Get()->PreRender();

	Device::Get()->Clear();

	Font::Get()->GetDC()->BeginDraw();

	Environment::Get()->SetViewport();
	Environment::Get()->SetProjection();

	SceneManager::Get()->Render();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (showFPS) {
		string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
		Font::Get()->RenderText(fps, { 100.0f, WIN_HEIGHT - 20.0f });
	}

	CAM->RenderUI();

	uiViewBuffer->SetVS(1);
	SceneManager::Get()->PostRender();

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

	SceneManager::Get();
	//pokemon
	ItemDataManager::Get();
	DungeonDataManager::Get();
	SkillDataManager::Get();
	UnitManager::Get();
	PokemonUIManager::Get();

	ClearData::Get();
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

	SceneManager::Delete();
	//pokemon
	ClearData::Delete();

	UnitManager::Delete();
	ItemDataManager::Delete();
	DungeonDataManager::Delete();
	SkillDataManager::Delete();
	PokemonUIManager::Delete();
}
