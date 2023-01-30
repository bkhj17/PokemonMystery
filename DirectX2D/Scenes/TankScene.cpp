#include "Framework.h"
#include "TankScene.h"

TankScene::TankScene()
{
	gameTileMap = new GameTileMap("TextData/TestMap.map");
	astar = new AStar(gameTileMap);

	tank = new Tank();
	tank->Pos() = { 160.0f, 160.0f };

	BulletManager::Get();

}

TankScene::~TankScene()
{
	delete tank;

	delete astar;
	delete gameTileMap;

	BulletManager::Delete();
}

void TankScene::Update()
{
	if (KEY_DOWN(VK_LBUTTON)) {
		int start = astar->FindCloseNode(tank->Pos());
		int end = astar->FindCloseNode(mousePos);
		astar->GetPath(start, end, tank->GetPath());
	}

	tank->Update();

	gameTileMap->PushObject(tank->GetCollider());
}

void TankScene::Render()
{
	gameTileMap->Render();
	astar->Render();

	tank->Render();


}

void TankScene::PostRender()
{
	tank->PostRender();
}
