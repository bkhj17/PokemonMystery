#pragma once
#include "UIWindow.h"
class YesNoUI : public UIWindow
{
public:
	YesNoUI(Vector2 size, Vector2 pos);
	~YesNoUI();

	virtual void Init() override;
	void Update() override;
	void PostRender() override;

	void SetYesEvent(string key) { yesEventKey = key; }

	virtual void RenderCursor() override;
private:
	wstring yes = L"예";
	wstring no = L"아니오";

	string yesEventKey = "";

	Quad* cQuad = nullptr;
	int cursor = 0;
};

