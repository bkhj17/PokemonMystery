#include "Framework.h"

ClearUI::ClearUI()
	: UIWindow(Vector2(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.8f), Vector2(CENTER_X, CENTER_Y))
{
}

void ClearUI::PostRender()
{
	__super::PostRender();

	Font::Get()->RenderText(L"던전 클리어", Pos() + Vector2(25.0f, Half().y - 80.0f));

	string output = "던전 : " + ClearData::Get()->GetDungeonName();
	Font::Get()->RenderTextLeft(output, Pos() + Vector2(-Half().x + 80.0f, Half().y - 150.0f));

	output = "도달 층계 : " + ClearData::Get()->GetFloorStr();
	Font::Get()->RenderTextLeft(output, Pos() + Vector2(-Half().x + 80.0f, Half().y - 200.0f));

	output = "플레이어 : " + ClearData::Get()->GetPlayerName();
	Font::Get()->RenderTextLeft(output, Pos() + Vector2(-Half().x + 80.0f, Half().y - 280.0f));

	output = "레벨 : " + to_string(ClearData::Get()->GetPlayerLevel());
	Font::Get()->RenderTextLeft(output, Pos() + Vector2(-Half().x + 80.0f, Half().y - 330.0f));

	output = "축하합니다!";
	Font::Get()->RenderText(output, Pos() + Vector2(25.0f, -Half().y + 80.0f));
}
