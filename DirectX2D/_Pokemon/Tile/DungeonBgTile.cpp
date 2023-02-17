#include "Framework.h"

DungeonBgTile::DungeonBgTile(Tile::Data data)
	: Quad(data.textureFile), data(data)
{
	localPosition = data.pos;
	localRotation.z = data.angle;

	cutSize = GetTexture()->GetSize() / BgTileManager::Get()->GRID_SIZE;
	size = cutSize;
	UpdateVertices();
}

DungeonBgTile::DungeonBgTile(Tile::Data data, Vector2 size)
	: Quad(data.textureFile, size), data(data)
{
	SetTexture(data.textureFile);
	localPosition = data.pos;
	localRotation.z = data.angle;

	cutSize = GetTexture()->GetSize() / BgTileManager::Get()->GRID_SIZE;
}

void DungeonBgTile::UpdateGrid(int flag)
{
	gridFrag = flag;

	auto gridCoord = BgTileManager::Get()->GetGrid(flag);
	Vector2 uvSize = cutSize / GetTexture()->GetSize();
	Vector2 startUV = uvSize * gridCoord;

	ModifyUV(startUV, startUV + uvSize);
}

bool DungeonBgTile::IsRoom()
{
	if (BgTileManager::Get()->GetTileType(texture) == BgTileManager::WALL)
		return false;

	return BgTileManager::Get()->IsRoom(gridFrag);
}

void DungeonBgTile::SetData(Tile::Data data)
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
