#pragma once
#include "../Object/DungeonObject.h"

class ItemObject : public DungeonObject
{
	ItemObject(string key, Vector2 size);
	~ItemObject() = default;

	// DungeonObject��(��) ���� ��ӵ�
	virtual void Update() override;


private:
	class ItemData* data;
};

