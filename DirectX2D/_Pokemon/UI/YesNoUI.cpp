#include "Framework.h"
#include "YesNoUI.h"
#include "PokemonUIManager.h"

YesNoUI::YesNoUI(Vector2 size, Vector2 pos)
	: UIWindow(size, pos)
{
	cQuad = new Quad(L"Textures/pokemon/UI/Cursor.png");
	cQuad->SetParent(this);
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
	if (KEY_DOWN(VK_DOWN))
		cursor = (++cursor) % 2;
	
	if (KEY_DOWN(VK_UP))
		cursor = cursor == 0 ? 1 : 0;

	float cPosY = cursor == 0 ? 30.0f : -15.0f;
	cQuad->Pos() = { -Half().x + 30.0f, cPosY };
	cQuad->UpdateWorld();

	if (KEY_DOWN('Z')) {
		switch (cursor)
		{
		case 0:
			Observer::Get()->ExecuteEvent(yesEventKey);
		case 1:
			isActive = false;
		}
		return;
	}

	if (KEY_DOWN('X')) {
		isActive = false;
		return;
	}

	UpdateWorld();
}

void YesNoUI::PostRender()
{
	__super::PostRender();
	
	Font::Get()->RenderText(yes, { Pos().x, Pos().y + 30.0f});
	Font::Get()->RenderText(no, { Pos().x, Pos().y - 15.0f});
	

}

void YesNoUI::RenderCursor()
{
	cQuad->PostRender();
}
