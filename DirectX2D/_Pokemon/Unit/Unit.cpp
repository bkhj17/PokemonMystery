#include "Framework.h"
#include "Unit.h"
#include "UnitMovement.h"

Unit::Unit(Vector2 size)
{
	this->size = size;

	animObject = new AnimObject();
	animObject->SetParent(this);

	collider = new RectCollider(size);
	collider->SetParent(this);

	movement = new UnitMovement();
	movement->SetOwner(this);
}

Unit::~Unit()
{
	delete animObject;
	delete collider;
	delete movement;
}

void Unit::Update()
{


	movement->Update();
	


	animObject->Update();


	UpdateWorld();
}

void Unit::Render()
{
	__super::SetRender();
	//animObject->Render();
	collider->Render();
}

void Unit::SetClipData()
{
}

void Unit::UpdateWorld()
{
	__super::UpdateWorld();
	animObject->UpdateWorld();
	collider->UpdateWorld();
}
