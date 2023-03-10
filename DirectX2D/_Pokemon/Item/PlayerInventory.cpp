#include "Framework.h"

bool PlayerInventory::InputItem(ItemData* item)
{
	if (IsFull())
		return false;

	LogManager::Get()->InsertLog(item->name + "을/를 가방에 넣었다.");
	itemList.push_back(item);
	return true;
}

void PlayerInventory::UseItem(UINT i)
{
	LogManager::Get()->InsertLog(itemList[i]->name + "을 사용했다.");
	itemList[i]->Use(UnitManager::Get()->GetPlayer());
	OutputItem(i);
}

ItemData* PlayerInventory::OutputItem(UINT i)
{
	if (i >= itemList.size() || i < 0)
		return nullptr;

	ItemData* data = itemList[i];
	itemList.erase(itemList.begin() + i);
	return data;
}
