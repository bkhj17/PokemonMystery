#include "Framework.h"
#include "MessageUI.h"

MessageUI::MessageUI()
	: UIWindow(0.0f, WIN_WIDTH, WIN_HEIGHT * 0.3f, 0.0f)
{
}

MessageUI::~MessageUI()
{
}

void MessageUI::PostRender()
{
	__super::PostRender();

	Font::Get()->RenderText(message, Vector2(Pos().x + 50.0f, Pos().y + 50.0f));
}
