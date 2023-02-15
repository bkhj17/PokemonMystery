#include "Framework.h"
#include "DungeonObjTile.h"
#include "../Unit/Unit.h"
#include "../Control/Controller.h"
#include "../UI/PokemonUIManager.h"
#include "../UI/YesNoUI.h"

DungeonObjTile::DungeonObjTile(string type, Tile::Data data, Vector2 size)
	: type(type), Tile(data, size)
{
	//밟은 유닛을 알아야만 한다......
	this->event = nullptr;
	paramEvent = nullptr;
	if (type == "Stair") {
		paramEvent = [](void* ptr) {
			Unit* unit = (Unit*)ptr;
			//밟은 Unit을 알아야 한다
			//Unit->Controller를 받아야 한다
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
			//적이라면 무시
			if (unit->GetController()->GetTag() == "Enemy")
				return;

			unit->SetDown(0);
			//밟은 유닛이 플레이어거나 동료라면 상태이상 제거
		};
	}

	object = nullptr;
}

void DungeonObjTile::Render()
{
	Quad::Render();
}
