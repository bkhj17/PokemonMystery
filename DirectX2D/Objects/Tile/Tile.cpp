#include "Framework.h"
#include "Tile.h"

Tile::Tile(Data data)
	: Button(data.textureFile), data(data)
{
	localPosition = data.pos;
	localRotation.z = data.angle;
}

Tile::~Tile()
{
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
