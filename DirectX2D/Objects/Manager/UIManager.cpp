#include "Framework.h"
#include "UIManager.h"

UIManager::UIManager()
{
	AddPanel("Test1", L"Textures/UI/Panel.png");
	panels["Test1"]->Pos() = { CENTER_X, CENTER_Y };
	AddPanel("Test2", L"Textures/UI/Panel.png");
	panels["Test2"]->Pos() = { CENTER_X - 50.0f, CENTER_Y };
	AddPanel("Test3", L"Textures/UI/Panel.png");
	panels["Test3"]->Pos() = { CENTER_X + 50.0f, CENTER_Y };
	AddPanel("Test4", L"Textures/UI/Panel.png");
	panels["Test4"]->Pos() = { CENTER_X + 50.0f, CENTER_Y - 50.0f };

}

UIManager::~UIManager()
{
	for (auto& panel : panels)
		delete panel.second;
	
}

void UIManager::Update()
{
	if (KEY_DOWN(VK_ESCAPE)) {
		HideFront();
	}

	for (auto& panel : panels) {
		panel.second->Update();
	}

	SelectUI();
	if (!popupList.empty()) {
		popupList.back()->Drag();
	}
}

void UIManager::Render()
{
	for (auto& panel : popupList)
		panel->Render();

	for (auto panel : panels)
		panel.second->RenderUI();
}

void UIManager::AddPanel(string key, wstring panelTextureFile, wstring cancelBtnTextureFile)
{
	if (panels.count(key) > 0)
		return;

	Panel * panel = new Panel(panelTextureFile, cancelBtnTextureFile);
	panel->SetTag(key);
	panel->Load();

	panels[key] = panel;
}

void UIManager::Show(string key)
{
	if (panels.count(key) == 0)
		return;

	Hide(key);

	panels[key]->SetActive(true);

	popupList.push_back(panels[key]);
	
}

void UIManager::Hide(string key)
{
	for (auto it = popupList.rbegin(); it != popupList.rend();) {
		if (*it == panels[key]) {
			popupList.erase(--it.base());
			return;
		}
		else it++;
	}
	/*
	for (auto it = popupList.begin(); it != popupList.end();) {
		if (*it == panels[key]) {
			it = popupList.erase(it);
			break;
		}
		else it++;
	}
	*/
}

void UIManager::HideFront()
{
	if (!popupList.empty()) {
		popupList.back()->SetActive(false);
		popupList.pop_back();
	}
}

void UIManager::SelectUI()
{
	if (popupList.empty())
		return;

	if (!KEY_DOWN(VK_LBUTTON))
		return;

	for (auto rit = popupList.rbegin(); rit != popupList.rend(); rit++) {
		if ((*rit)->GetCollider()->IsPointCollision(mousePos)) {
			if(rit != popupList.rbegin())
				Show((*rit)->tag);
			return;
		}
	}
}
