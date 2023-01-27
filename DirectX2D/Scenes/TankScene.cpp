#include "Framework.h"
#include "TankScene.h"

TankScene::TankScene()
{
	gameTileMap = new GameTileMap("TextData/TestMap.map");

	tank = new Tank();
	tank->Pos() = { 120.0f, 80.0f };

	BulletManager::Get();

}

TankScene::~TankScene()
{
	delete gameTileMap;
	delete tank;

	BulletManager::Delete();
}

void TankScene::Update()
{
	tank->Update();

	gameTileMap->PushObject(tank->GetCollider());
}

void TankScene::Render()
{
	gameTileMap->Render();
	tank->Render();
}

void TankScene::PostRender()
{
	tank->PostRender();
}
