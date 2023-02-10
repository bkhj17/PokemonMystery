#pragma once

class ItemObject;
class ItemObjectManager : public Singleton<ItemObjectManager>
{
private:
	friend class Singleton;
	ItemObjectManager();
	~ItemObjectManager();

public:
	void InitItem(string key, POINT point);
	
	void Update();
	void Render();

	ItemObject* ItemOnPoint(POINT point);
	
	void PickUp(void* ptr);
private:
	int numActive = 0;
	vector<ItemObject*> itemObjs;
};

