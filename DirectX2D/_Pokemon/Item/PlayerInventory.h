#pragma once

struct ItemData;
class PlayerInventory : public Singleton<PlayerInventory>
{
private:
	friend class Singleton;
	PlayerInventory() = default;
	~PlayerInventory() = default;
public:
	bool InputItem(ItemData* item);
	bool IsFull() { return itemList.size() == maxItemNum; }

	ItemData* OutputItem(int i);

private:
	//�÷��̾ ���� ������ ����Ʈ
	int maxItemNum = 20;
	deque<ItemData*> itemList;
};

