#pragma once
#include "UIWindow.h"
class MessageUI : public UIWindow
{
public:
	
	MessageUI();
	~MessageUI() = default;

	void PostRender() override;

	void SetMessage(string str) { message = str; }
private:
	string message;
};

