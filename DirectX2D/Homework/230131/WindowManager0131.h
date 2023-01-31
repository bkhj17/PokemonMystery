#pragma once
class Window0131;
class WindowManager0131
{
public:
	WindowManager0131();
	~WindowManager0131();

	void Update();
	void Render();

	void WindowOpenByNum(int n);
private:
	void WindowOpen(Window0131* window);
	void CloseWindow();

private:
	vector<Window0131*> windowPool;
	vector<Window0131*> openWindow;
};

