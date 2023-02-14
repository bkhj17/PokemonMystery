#include "Framework.h"
#include "YesNoUI.h"
#include "PokemonUIManager.h"

YesNoUI::YesNoUI(Vector2 size, Vector2 pos)
	: UIWindow(size, pos)
{
	cQuad = new Quad(L"Textures/pokemon/UI/Cursor.png");
	cQuad->SetParent(this);

	maxCursor = 2;
}

YesNoUI::~YesNoUI()
{
	delete cQuad;
}

void YesNoUI::Init()
{
	__super::Init();
	cursor = 0;
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
			isActive = false;
			break;
		case 1:
			isActive = false;
			break;
		}
		return;
	}
}

void YesNoUI::PostRender()
{
	__super::PostRender();
	
	Font::Get()->RenderText(yes, Pos() + YES_POS);
	Font::Get()->RenderText(no, Pos() + NO_POS);

	RenderCursor();
}

