#include "Framework.h"
#include "Scene0131.h"
#include "WindowManager0131.h"

Scene0131::Scene0131()
{
	windowManager = new WindowManager0131;

	buttons.resize(5);
	for (int i = 0; i < 5; i++) {
		buttons[i] = new Button(Vector2(50.0f, 50.0f));
		buttons[i]->SetTexture(Texture::Add(L"Textures/WindowTest/b1.png"));
		buttons[i]->Pos() = { CENTER_X - 125.0f + 50.0f * i, buttons[i]->Half().y };
		buttons[i]->UpdateWorld();
		buttons[i]->SetEvent(bind(&WindowManager0131::WindowOpenByNum, windowManager, i));
	}
}

Scene0131::~Scene0131()
{
	delete windowManager;
}

void Scene0131::Update()
{
	windowManager->Update();

	if (KEY_DOWN(VK_ESCAPE)) {
		Observer::Get()->ExecuteEvent("CloseWindow");
	}

	for (auto button : buttons)
		button->Update();
}

void Scene0131::Render()
{
}

void Scene0131::PostRender()
{
	windowManager->Render();

	for (auto button : buttons)
		button->Render();
}
