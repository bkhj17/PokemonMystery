#include "Framework.h"
#include "DungeonScene.h"
#include "../Tile/DungeonTileMap.h"
#include "../Tile/BgTileManager.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitMovement.h"

DungeonScene::DungeonScene()
{
	tileMap = new DungeonTileMap("TextData/TinyWood1.map");
	CAM->SetLeftBottom(tileMap->LeftBottom());
	CAM->SetRightTop(tileMap->RightTop());

	Vector2 testStart = tileMap->PointToPos({23, 14});

	testUnit = new Unit(Vector2(50, 60));
	testUnit->GetMovement()->SetTargetPos(testStart, 0.0f);

	CAM->SetTarget(testUnit);
}

DungeonScene::~DungeonScene()
{
	delete testUnit;
	delete tileMap;
	BgTileManager::Delete();
}

void DungeonScene::Update()
{
	if (!testUnit->GetMovement()->IsMoving()) {
		int x = 0, y = 0;
		if (KEY_PRESS(VK_DOWN)) --y;
		if (KEY_PRESS(VK_UP))	++y;
		if (KEY_PRESS(VK_LEFT))	--x;
		if (KEY_PRESS(VK_RIGHT))++x;

		if (x != 0 || y != 0) {
			Vector2 unitPos = testUnit->GlobalPos();

			POINT unitPoint = tileMap->PosToPoint(unitPos);
			Vector2 destPos = {};
			if(tileMap->SetMove(unitPoint.x, unitPoint.y, x, y, destPos))
				testUnit->GetMovement()->SetTargetPos(destPos);
		}
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
