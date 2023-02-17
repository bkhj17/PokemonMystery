#pragma once
#include "UIWindow.h"
class LogUi : public UIWindow
{
public:
	LogUi();
	~LogUi() = default;

	void Update();
	void PostRender();
	void Hide() { renderTime = 0.0f; }
private:
	void SetRenderTime(float time) { renderTime = max(renderTime, time); }

	float LineY(int lineNum);
private:
	float renderTime = 0.0f;


};

