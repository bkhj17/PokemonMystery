#include "Framework.h"
#include "UIWindow.h"

UIWindow::UIWindow(Vector2 size, Vector2 pos)
	: Quad(size)
{
	localPosition = pos;
}

UIWindow::UIWindow(float left, float right, float up, float down)
	: Quad(Vector2(right - left, up - down))
{

	localPosition = { (right + left) / 2, (up + down) / 2 };
}

void UIWindow::Init()
{
}
