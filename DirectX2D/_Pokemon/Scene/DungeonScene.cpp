#include "Framework.h"
#include "DungeonScene.h"
#include "../Tile/DungeonTileMap.h"

DungeonScene::DungeonScene()
{
	tileMap = new DungeonTileMap("TextData/TinyWood1.map");
	CAM->SetLeftBottom(tileMap->LeftBottom());
	CAM->SetRightTop(tileMap->RightTop());

}

DungeonScene::~DungeonScene()
{
	delete tileMap;
}

void DungeonScene::Update()
{
}

void DungeonScene::Render()
{
	tileMap->Render();
}

void DungeonScene::PostRender()
{
}
