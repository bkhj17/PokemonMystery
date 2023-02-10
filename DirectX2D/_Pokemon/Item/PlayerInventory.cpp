#include "Framework.h"
#include "PlayerInventory.h"
#include "../Data/ItemDataManager.h"

bool PlayerInventory::InputItem(ItemData* item)
{
	if (IsFull())
		return false;

	itemList.push_back(item);
	return true;
}

ItemData* PlayerInventory::OutputItem(int i)
{
	if (i >= itemList.size() || i < 0)
		return nullptr;

	ItemData* data = itemList[i];
	itemList.erase(itemList.begin() + i);
	return data;
}
