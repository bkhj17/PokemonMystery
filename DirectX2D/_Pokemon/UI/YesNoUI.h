#pragma once
#include "UIWindow.h"
class YesNoUI : public UIWindow
{
	const Vector2 YES_POS = { 0.0f, 25.0f };
	const Vector2 NO_POS = { 0.0f, -15.0f };

public:
	YesNoUI(Vector2 size, Vector2 pos);
	~YesNoUI();

	virtual void Init() override;
	void Update() override;
	void PostRender() override;

	void SetYesEvent(string key) { yesEventKey = key; }

private:
	wstring yes = L"예";
	wstring no = L"아니오";

	string yesEventKey = "";
};

