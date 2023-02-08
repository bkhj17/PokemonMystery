#pragma once
#include "../Object/DungeonObject.h"

class ItemObject : public DungeonObject
{
	ItemObject(string key, Vector2 size);
	~ItemObject() = default;

	// DungeonObject을(를) 통해 상속됨
	virtual void Update() override;


private:
	class ItemData* data;
};

