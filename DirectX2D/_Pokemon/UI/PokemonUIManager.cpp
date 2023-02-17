#include "Framework.h"
#include "PokemonUIManager.h"
#include "UIWindow.h"
#include "YesNoUI.h"
#include "StatusUI.h"
#include "MiniMap.h"
#include "SkillUI.h"
#include "MessageUI.h"
#include "ItemUi.h"
#include "LogUi.h"

PokemonUIManager::PokemonUIManager()
{
	statusUI = new StatusUI();
	statusUI->Pos() = { CENTER_X, WIN_HEIGHT - 40.0f };
	statusUI->UpdateWorld();

	totalUI["Skill"] = new SkillUI();
	totalUI["YesNo"] = new YesNoUI({ 150, 100 }, { WIN_WIDTH - 150, CENTER_Y });
	totalUI["Item"] = new ItemUi();



	miniMap = new MiniMap();
	logUI = new LogUi();
}

PokemonUIManager::~PokemonUIManager()
{
	delete statusUI;
	
	for (auto& ui : totalUI)
		delete ui.second;
	delete miniMap;
}

void PokemonUIManager::Update()
{
	while (!openned.empty() && !openned.back()->Active())
		openned.pop_back();
	
	if (!openned.empty())
		openned.back()->Update();

	logUI->Update();
}

void PokemonUIManager::PostRender()
{
	statusUI->PostRender();

	for (auto window : openned)
		window->PostRender();

	if (!openned.empty())
		openned.back()->RenderCursor();
	else {
		miniMap->PostRender();
		logUI->PostRender();
	}
}

void PokemonUIManager::OpenMenu()
{
	OpenUI("Menu");
}

void PokemonUIManager::CloseAll()
{
	openned.clear();
}

UIWindow* PokemonUIManager::OpenUI(string key)
{
	if (totalUI.find(key) == totalUI.end())
		return nullptr;

	openned.push_back(totalUI[key]);
	openned.back()->Init();

	logUI->Hide();

	return openned.back();
}

void PokemonUIManager::CloseUI()
{
	if (openned.empty())
		return;
	openned.pop_back();
}

void PokemonUIManager::CloseUI(string key)
{
	if (totalUI.find(key) == totalUI.end())
		return;

	totalUI[key]->Close();
}

bool PokemonUIManager::IsActing()
{
	return !openned.empty();
}
