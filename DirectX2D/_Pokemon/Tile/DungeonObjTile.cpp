#include "Framework.h"
#include "DungeonObjTile.h"
#include "../Unit/Unit.h"
#include "../Control/Controller.h"
#include "../UI/PokemonUIManager.h"
#include "../UI/YesNoUI.h"

DungeonObjTile::DungeonObjTile(string type, Tile::Data data, Vector2 size)
	: type(type), Tile(data, size)
{
	//���� ������ �˾ƾ߸� �Ѵ�......
	this->event = nullptr;
	paramEvent = nullptr;
	if (type == "Stair") {
		paramEvent = [](void* ptr) {
			Unit* unit = (Unit*)ptr;
			//���� Unit�� �˾ƾ� �Ѵ�
			//Unit->Controller�� �޾ƾ� �Ѵ�
			if (unit->GetController()->GetTag() != "Player")
				return;

			auto ynUi = (YesNoUI*)PokemonUIManager::Get()->OpenUI("YesNo");
			if (ynUi == nullptr)
				return;
			ynUi->SetYesEvent("ShiftNextFloor");
		};
	}
	else if (type == "Refresh") {
		paramEvent = [](void* ptr) {
			Unit* unit = (Unit*)ptr;
			//���̶�� ����
			if (unit->GetController()->GetTag() == "Enemy")
				return;

			unit->SetDown(0);
			//���� ������ �÷��̾�ų� ������ �����̻� ����
		};
	}

	object = nullptr;
}

void DungeonObjTile::Render()
{
	Quad::Render();
}
