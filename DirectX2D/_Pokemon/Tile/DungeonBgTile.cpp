#include "Framework.h"
#include "DungeonBgTile.h"
#include "BgTileManager.h"

DungeonBgTile::DungeonBgTile(Tile::Data data)
	: Tile(data)
{
	cutSize = GetTexture()->GetSize() / BgTileManager::Get()->GRID_SIZE;
	size = cutSize;
	UpdateVertices();
}

DungeonBgTile::DungeonBgTile(Tile::Data data, Vector2 size)
	: Tile(data, size)
{
	cutSize = GetTexture()->GetSize() / BgTileManager::Get()->GRID_SIZE;
}

void DungeonBgTile::Render()
{
	__super::Render();

	collider->Render();

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
