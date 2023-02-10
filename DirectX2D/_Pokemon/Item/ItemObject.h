#pragma once
#include "../Object/DungeonObject.h"

struct ItemData;
class ItemObject : public DungeonObject
{
private:
	const Vector2 SIZE = { 80.0f, 80.0f };

public:
	ItemObject();
	~ItemObject();

	void SetData(string key);
	ItemData* GetData() { return data; }
	// DungeonObject을(를) 통해 상속됨
	virtual void Update() override;
	virtual void UpdateWorld() override;
	virtual void Render() override;
private:
	virtual bool IsCollide() override;
private:
	ItemData* data = nullptr;
	Quad* quad;
};