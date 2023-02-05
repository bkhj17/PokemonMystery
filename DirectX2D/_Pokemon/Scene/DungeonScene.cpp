#include "Framework.h"
#include "DungeonScene.h"
#include "../Tile/DungeonTileMap.h"
#include "../Tile/BgTileManager.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitMovement.h"
#include "../Control/Controller.h"

DungeonScene::DungeonScene()
{
	tileMap = new DungeonTileMap("TextData/TinyWood1.map");
	CAM->SetLeftBottom(tileMap->LeftBottom());
	CAM->SetRightTop(tileMap->RightTop());

	Observer::Get()->AddGetEvent("CallTileMap", bind(&DungeonScene::CallTileMap, this, placeholders::_1));
	
	testUnit = new Unit(new Controller, Vector2(50, 60));
	testUnit->SetPos(24, 14);
	CAM->SetTarget(testUnit);
}

DungeonScene::~DungeonScene()
{
	delete tileMap;
	BgTileManager::Delete();
}

void DungeonScene::Update()
{
	switch (actState)
	{
	case DungeonScene::ENTER_DUNGEON:
		actState = WAIT_COMMAND;
		break;
	case DungeonScene::WAIT_COMMAND:
	{
		testUnit->GetController()->SetCommand();
		if (testUnit->IsActing())
			actState = ACTING;
	}
		break;
	case DungeonScene::ACTING:
		
		//후에 유닛 목록으로 교체
		if (testUnit->IsActing())
			break;

		actState = WAIT_COMMAND;
		break;
	case DungeonScene::FLOOR_MOVE:
		break;
	case DungeonScene::PLAYER_DEAD:
		break;
	default:
		break;
	}
	testUnit->Update();
}

void DungeonScene::Render()
{
	tileMap->Render();
	testUnit->Render();
}

void DungeonScene::PostRender()
{
}