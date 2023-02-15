#include "Framework.h"
#include "MiniMap.h"
#include "../Tile/DungeonTileMap.h"
#include "../Tile/DungeonBgTile.h"
#include "../Unit/Unit.h"
#include "../Control/Controller.h"

MiniMap::MiniMap()
{
	quad = new Quad(Vector2(20.0f, 20.0f));
	gridBuffer = new IntValueBuffer;
	quad->SetPixelShader(L"MiniMapShader.hlsl");

	Observer::Get()->AddParamEvent("MoveEnd", bind(&MiniMap::UpdateMap, this, placeholders::_1));
}

MiniMap::~MiniMap()
{
	delete quad;
	delete gridBuffer;
}

void MiniMap::Init()
{
	//가지고 있던 정보를 전부 없앤다
	Clear();

}

void MiniMap::UpdateMap(void* unitPtr)
{
	Unit* unit = (Unit*)unitPtr;
	if (!unit)
		return;

	if (unit->GetController()->GetTag() != "Player")
		return;

	DungeonTileMap* tileMap = nullptr;
	Observer::Get()->ExecuteGetEvent("CallTileMap", (void**)&tileMap);

	if (!tileMap)
		return;
	
	vector<pair<int, int>> detectPoints = tileMap->DetectableTiles(unit->GetPoint());
	
	for (const auto& detect : detectPoints)
		info[detect] = tileMap->GetBgTile({ detect.first, detect.second })->GetGridFlag();
}

void MiniMap::PostRender()
{
	for (auto& i : info) {
		quad->Pos() = { i.first.first * quad->GetSize().x,  i.first.second * quad->GetSize().y };
		quad->UpdateWorld();
		gridBuffer->Get()[0] = i.second;
		gridBuffer->SetPS(5);
		quad->PostRender();
	}

}

void MiniMap::Clear()
{
	info.clear();
}
