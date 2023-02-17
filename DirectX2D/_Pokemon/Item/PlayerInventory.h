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

	void UseItem(UINT i);
	ItemData* OutputItem(UINT i);

	vector<ItemData*>& GetList() { return itemList; }
private:
	//�÷��̾ ���� ������ ����Ʈ
	int maxItemNum = 20;
	vector<ItemData*> itemList;
};

