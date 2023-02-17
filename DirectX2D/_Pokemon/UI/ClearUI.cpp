#include "Framework.h"

ClearUI::ClearUI()
	: UIWindow(Vector2(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.8f), Vector2(CENTER_X, CENTER_Y))
{
}

void ClearUI::PostRender()
{
	__super::PostRender();

	Font::Get()->RenderText(L"���� Ŭ����", Pos() + Vector2(25.0f, Half().y - 80.0f));

	string output = "���� : " + ClearData::Get()->GetDungeonName();
	Font::Get()->RenderTextLeft(output, Pos() + Vector2(-Half().x + 80.0f, Half().y - 150.0f));

	output = "���� ���� : " + ClearData::Get()->GetFloorStr();
	Font::Get()->RenderTextLeft(output, Pos() + Vector2(-Half().x + 80.0f, Half().y - 200.0f));

	output = "�÷��̾� : " + ClearData::Get()->GetPlayerName();
	Font::Get()->RenderTextLeft(output, Pos() + Vector2(-Half().x + 80.0f, Half().y - 280.0f));

	output = "���� : " + to_string(ClearData::Get()->GetPlayerLevel());
	Font::Get()->RenderTextLeft(output, Pos() + Vector2(-Half().x + 80.0f, Half().y - 330.0f));

	output = "�����մϴ�!";
	Font::Get()->RenderText(output, Pos() + Vector2(25.0f, -Half().y + 80.0f));
}
