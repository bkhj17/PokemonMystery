#include "Framework.h"
#include "TankScene.h"

TankScene::TankScene()
{
	gameTileMap = new GameTileMap("TextData/TestMap.map");
	astar = new AStar(gameTileMap);

	isaac = new Isaac;
	isaac->Pos() = { 550.0f, 160.0f };

	tank = new Tank(astar);
	tank->Pos() = { 160.0f, 160.0f };
	tank->SetTarget(isaac);

	Observer::Get()->ExecuteParamEvent("TankPatrolSetTarget", isaac);
	Observer::Get()->ExecuteParamEvent("TankTraceSetTarget", isaac);
}

TankScene::~TankScene()
{
	delete tank;

	delete isaac;

	delete astar;
	delete gameTileMap;

}

void TankScene::Update()
{
	if (KEY_DOWN(VK_LBUTTON)) {
		int start = astar->FindCloseNode(tank->Pos());
		int end = astar->FindCloseNode(mousePos);
		//astar->GetPath(start, end, tank->GetPath());
	}

	astar->Update();
	tank->Update();
	isaac->Update();
	//gameTileMap->PushObject(tank->GetCollider());

	gameTileMap->PushRect(isaac);
}

void TankScene::Render()
{
	gameTileMap->Render();
	astar->Render();

	tank->Render();

	isaac->Render();
}

void TankScene::PostRender()
{
	tank->PostRender();
}
