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

void Tile::SetTexture(wstring file)
{
	__super::SetTexture(file);
	data.textureFile = file;
}

void Tile::SetAngle(float angle)
{
	data.angle = localRotation.z = angle;
}
