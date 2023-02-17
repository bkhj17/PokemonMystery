#include "Framework.h"

YesNoUI::YesNoUI(Vector2 size, Vector2 pos)
	: UIWindow(size, pos)
{
	maxCursor = 2;
	messageUI = new MessageUI();
	messageUI->SetMessage("계단을 발견했다. 이동하시겠습니까?");
}

YesNoUI::~YesNoUI()
{
	delete messageUI;
}

void YesNoUI::Init()
{
	__super::Init();
}

void YesNoUI::Update()
{
	__super::Update();
	if (!isActive)
		return;

	float cPosY = -5.0f + (cursor == 0 ? YES_POS.y : NO_POS.y);
	cQuad->Pos() = { -Half().x + 30.0f, cPosY };
	cQuad->UpdateWorld();

	if (KEY_DOWN('Z')) {
		switch (cursor)
		{
		case 0:
			Observer::Get()->ExecuteEvent(yesEventKey);
			Close();
			break;
		case 1:
			isActive = false;
			Close();
		}
		return;
	}
}

void YesNoUI::PostRender()
{
	messageUI->PostRender();

	__super::PostRender();
	
	Font::Get()->RenderText(yes, Pos() + YES_POS);
	Font::Get()->RenderText(no, Pos() + NO_POS);

	RenderCursor();
}

void YesNoUI::Close()
{
	__super::Close();
}

