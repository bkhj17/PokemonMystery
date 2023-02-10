#include "Framework.h"
#include "ItemObject.h"
#include "../Unit/UnitMovement.h"
#include "../Data/ItemDataManager.h"

ItemObject::ItemObject()
	: DungeonObject(SIZE)
{
	isActive = false;

	quad = new Quad(size);
	quad->SetParent(this);
}

ItemObject::~ItemObject()
{
	delete quad;
}

void ItemObject::SetData(string key)
{
	//ItemManager에서 ItemData불러오기 
	data = ItemDataManager::Get()->GetData(key);
	if (data == nullptr) {
		isActive = false;
		return;
	}

	quad->SetTexture(data->textureFile);
	quad->SetSize(quad->GetTexture()->GetSize());
}

void ItemObject::Update()
{
	if (!isActive)
		return;

	movement->Update();
	UpdateWorld();
}

void ItemObject::UpdateWorld()
{
	__super::UpdateWorld();
	quad->UpdateWorld();
}

void ItemObject::Render()
{
	if (!isActive)
		return;

	__super::Render();
	quad->Render();
}

bool ItemObject::IsCollide()
{
	return false;
}
