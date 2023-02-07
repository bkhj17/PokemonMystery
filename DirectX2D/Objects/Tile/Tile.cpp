#include "Framework.h"
#include "Tile.h"

Tile::Tile(Data data)
	: Button(data.textureFile), data(data)
{
	localPosition = data.pos;
	localRotation.z = data.angle;
}

Tile::Tile(Data data, Vector2 size)
	: Button(size), data(data)
{
	SetTexture(data.textureFile);
	localPosition = data.pos;
	localRotation.z = data.angle;
}

void Tile::Update()
{
	if (!isActive)
		return;

	if (collider->IsPointCollision(CAM->ScreenToWorld(mousePos))) {
		colorBuffer->Get() = { 0, 0.5f, 0, 1.0f };
	} else
		colorBuffer->Get() = NONE_COLOR;

	UpdateWorld();
}

void Tile::SetTexture(wstring file)
{
	__super::SetTexture(file);
	data.textureFile = file;
}

void Tile::SetAngle(float angle)
{
	data.angle = localRotation.z = angle;
}

void Tile::SetData(Data data)
{
	this->data.textureFile = data.textureFile;
	this->data.pos = data.pos;
	this->data.angle = data.angle;
	this->data.type = data.type;

	SetTexture(data.textureFile);
	localPosition = data.pos;
	localRotation.z = data.angle;
	
	UpdateWorld();
}

void Tile::PushRect(RectCollider* collider)
{
	Vector2 overlap = {};
	if (collider->IsRectCollision(this->collider, &overlap)) {
		if (overlap.x > overlap.y) {
			bool isUp = collider->GetParent()->GlobalPos().y > GlobalPos().y;
			collider->GetParent()->Pos().y += isUp ? overlap.y : -overlap.y;
		}
		else {
			bool isRight = collider->GetParent()->GlobalPos().x > GlobalPos().x;
			collider->GetParent()->Pos().x += isRight ? overlap.x : -overlap.x;
		}

		collider->GetParent()->UpdateWorld();
	}
}
