#include "Framework.h"
#include "ItemObjectManager.h"
#include "ItemObject.h"
#include "../Data/ItemDataManager.h"
#include "../Unit/Unit.h"

ItemObjectManager::ItemObjectManager()
{
	itemObjs.resize(2000);
	for (auto& item : itemObjs)
		item = new ItemObject();

	Observer::Get()->AddParamEvent("MoveEnd",
		bind(&ItemObjectManager::PickUp, this, placeholders::_1)
	);
}

ItemObjectManager::~ItemObjectManager()
{
	for (auto item : itemObjs)
		delete item;
}

void ItemObjectManager::InitItem(string key, POINT point)
{	
	for (auto item : itemObjs)
	{
		if (item->Active())
			continue;

		item->SetActive(true);
		item->SetData(key);
		item->SetPoint(point);
		return;
	}
}

void ItemObjectManager::Clear()
{
	for (auto& item : itemObjs)
		item->SetActive(false);
}

void ItemObjectManager::Update()
{
	for (auto& item : itemObjs)
		item->Update();
}

void ItemObjectManager::Render()
{
	for (auto& item : itemObjs)
		item->Render();
}

ItemObject* ItemObjectManager::ItemOnPoint(POINT point)
{
	for (auto item : itemObjs) {
		if (!item->Active())
			continue;

		POINT itemPoint = item->GetPoint();
		if (itemPoint.x == point.x && itemPoint.y == point.y)
			return item;
	}

	return nullptr;
}

void ItemObjectManager::PickUp(void* ptr)
{
	Unit* unit = (Unit*)ptr;
	if (unit == nullptr)
		return;

	POINT unitPoint = unit->GetPoint();
	ItemObject* object = ItemOnPoint(unitPoint);
	if (object != nullptr) {
		unit->PickUpItem(object->GetData());
		object->SetActive(false);
	}

}
