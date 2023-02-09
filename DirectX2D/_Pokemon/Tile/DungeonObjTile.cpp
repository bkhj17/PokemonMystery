#include "Framework.h"
#include "DungeonObjTile.h"
#include "../Unit/Unit.h"
#include "../Control/Controller.h"

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

			Observer::Get()->ExecuteEvent("ShiftNextFloor");
		};
	}
	else if (type == "Refresh") {
		paramEvent = [](void* ptr) {
			Unit* unit = (Unit*)ptr;
			//���̶�� ����
			if (unit->GetController()->GetTag() == "Enemy")
				return;

			//���� ������ �÷��̾�ų� ������ �����̻� ����
		};
	}

	object = nullptr;
}

void DungeonObjTile::Render()
{
	Quad::Render();
}
