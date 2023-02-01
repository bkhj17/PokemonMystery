#include "Framework.h"
#include "UIScene.h"

UIScene::UIScene()
{
	tileMap = new GameTileMap("TextData/UIssac.map");
	isaac = new Isaac;
	isaac->Pos() = { CENTER_X, CENTER_Y };

	Vector2 rightTop = { 3000.0f, 3000.0f };//tileMap->RightTop();
	CAM->SetRightTop(rightTop);

	CAM->SetTarget(isaac);

	UIManager::Get()->Show("Test1");
	UIManager::Get()->Show("Test2");
	UIManager::Get()->Show("Test3");
	UIManager::Get()->Show("Test4");
}

UIScene::~UIScene()
{
	delete tileMap;
	delete isaac;
	UIManager::Delete();
}

void UIScene::Update()
{
	isaac->Update();
	UIManager::Get()->Update();
}

void UIScene::Render()
{
	tileMap->Render();
	isaac->Render();
}

void UIScene::PostRender()
{
	UIManager::Get()->Render();
}
