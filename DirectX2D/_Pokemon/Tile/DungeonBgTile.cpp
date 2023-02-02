#include "Framework.h"
#include "DungeonBgTile.h"
#include "BgTileManager.h"

DungeonBgTile::DungeonBgTile(Tile::Data data, Vector2 size)
	: Tile(data, size)
{
	cutSize = GetTexture()->GetSize() / BgTileManager::Get()->GRID_SIZE;
	
	BgTileManager::Get();
}

void DungeonBgTile::UpdateGrid(int flag)
{
	gridFrag = flag;

	if (IsRoom()) {
		colorBuffer->Get() = { 0.5f, 1.0f, 0.5f, 1.0f };
	}

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
