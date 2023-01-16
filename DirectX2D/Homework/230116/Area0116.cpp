#include "Framework.h"
#include "Area0116.h"

Area0116::Area0116(Vector2 size, Vector2 pos)
	: Quad(size, pos)
{
	SetTexture(L"Textures/Shooting/background.png");
}

Area0116::~Area0116()
{
}

Vector2 Area0116::LeftTop()
{
	return Vector2(GlobalPos().x - Half().x, GlobalPos().y + Half().y);
}

Vector2 Area0116::RightBottom()
{
	return Vector2(GlobalPos().x + Half().x, GlobalPos().y - Half().y);
}
