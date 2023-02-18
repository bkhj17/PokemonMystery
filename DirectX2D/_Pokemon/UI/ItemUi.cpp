#include "Framework.h"

ItemUi::ItemUi()
	: UIWindow({ WIN_WIDTH * 0.3f, WIN_HEIGHT * 0.8f }, {WIN_WIDTH * 0.3f, WIN_HEIGHT * 0.6f})
{
	maxCursor = CLOSE_CURSOR + 1;
}

void ItemUi::Update()
{
	if (!isActive)
		return;

	__super::Update();
	auto& itemList = PlayerInventory::Get()->GetList();

	//���� Ŀ���� �ش��ϴ� ��ų�� ��ȿ���� �ʴٸ� �������� �ѱ��
	if (itemList.empty()) {
		cursor = CLOSE_CURSOR;
	}
	if (KEY_DOWN(VK_DOWN)) {
		if (cursor >= itemList.size())
			cursor = CLOSE_CURSOR;
	}
	if (KEY_DOWN(VK_UP)) {
		if(cursor >= itemList.size() && cursor < CLOSE_CURSOR)
			cursor = (int)itemList.size() - 1;
	}
	//Ŀ�� ��ġ ����
	cQuad->Pos() = { CursorX(), LineY(cursor) };
	cQuad->UpdateWorld();

	if (KEY_DOWN('Z')) {
		if (cursor == CLOSE_CURSOR) {
			Close();
			return;
		}
		else {
			PlayerInventory::Get()->UseItem(cursor);
			Close();
		}
	}
}

void ItemUi::PostRender()
{
	__super::PostRender();
	RenderCursor();

	auto& itemList = PlayerInventory::Get()->GetList();

	for (int i = 0; i < itemList.size(); i++) {
		if (itemList[i] != nullptr)
			Font::Get()->RenderTextLeft(itemList[i]->name, Pos() + Vector2(NameX(), LineY(i)));
	}

	Font::Get()->RenderTextLeft(L"�ڷ�", Pos() + Vector2(-Half().x + 80.0f + cQuad->GetSize().x, LineY(CLOSE_CURSOR)));
}

float ItemUi::CursorX()
{
	return -Half().x + 80.0f;
}

float ItemUi::NameX()
{
	return -Half().x + 80.0f + cQuad->GetSize().x;
}

float ItemUi::LineY(int lineNum)
{
	return Half().y - 60.0f - lineNum * (cQuad->GetSize().y);
}
