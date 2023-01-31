#include "Framework.h"
#include "WindowManager0131.h"
#include "Window0131.h"

WindowManager0131::WindowManager0131()
{
	windowPool.resize(5);
	int n = 0;
	for (auto& window : windowPool) {
		window = new Window0131({ 500.0f, 400.0f });
		window->Pos() = { CENTER_X - 20.0f * n, CENTER_Y + 10.0f * n};
		window->SetTag("Window" + to_string(n++));
	}
	Observer::Get()->AddEvent("CloseWindow", bind(&WindowManager0131::CloseWindow, this));
}

WindowManager0131::~WindowManager0131()
{
	for (auto window : windowPool) {
		delete window;
	}
	windowPool.clear();
}

void WindowManager0131::Update()
{
	if (KEY_DOWN(VK_LBUTTON)) {
		Window0131* selectedWindow = nullptr;
		for (auto window : openWindow) {
			if (window->GetCollider()->IsPointCollision(mousePos))
				selectedWindow = window;
		}

		if (selectedWindow != nullptr)
			WindowOpen(selectedWindow);
	}

	if(!openWindow.empty())
		openWindow.back()->Update();
}

void WindowManager0131::Render()
{
	for (auto window : openWindow)
		window->Render();
}

void WindowManager0131::WindowOpenByNum(int n)
{
	if (n > windowPool.size())
		return;

	WindowOpen(windowPool[n]);
}

void WindowManager0131::WindowOpen(Window0131* window)
{
	for (auto it = openWindow.begin(); it != openWindow.end();) {
		if (*it == window) {
			openWindow.erase(it);
			break;
		}
		else {
			it++;
		}
	}
	
	window->SetActive(true);
	window->UpdateWorld();
	openWindow.push_back(window);
}

void WindowManager0131::CloseWindow()
{

	if (!openWindow.empty()) {
		openWindow.back()->SetActive(false);
		openWindow.pop_back();
	}
}
