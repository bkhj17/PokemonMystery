#include "Framework.h"
#include "PokemonUIManager.h"
#include "UIWindow.h"
#include "StatusUI.h"

PokemonUIManager::PokemonUIManager()
{
	statusUI = new StatusUI();
	statusUI->Pos() = { CENTER_X, WIN_HEIGHT - 40.0f };
	statusUI->UpdateWorld();

	UIWindow* testWindow = new UIWindow(Vector2(CENTER_X, CENTER_Y), Vector2(CENTER_X, CENTER_Y));
	totalUI["test"] = testWindow;
	openned.push_back(testWindow);
}

PokemonUIManager::~PokemonUIManager()
{
	delete statusUI;
	
	for (auto& ui : totalUI)
		delete ui.second;
}

void PokemonUIManager::Update()
{
	if(!openned.empty())
		openned.back()->Update();

}

void PokemonUIManager::PostRender()
{
	statusUI->PostRender();

	for (auto window : openned)
		window->PostRender();
}

void PokemonUIManager::OpenMenu()
{
	OpenUI("Menu");
}

void PokemonUIManager::CloseAll()
{
	totalUI.clear();
}

void PokemonUIManager::OpenUI(string key)
{
	if (totalUI.find(key) == totalUI.end())
		return;

	
	openned.push_back(totalUI[key]);
	openned.back()->Init();
}

void PokemonUIManager::CloseUI()
{
	if (openned.empty())
		return;
	openned.pop_back();
}
