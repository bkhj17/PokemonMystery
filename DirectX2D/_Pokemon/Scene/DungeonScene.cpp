#include "Framework.h"
#include "DungeonScene.h"
#include "../Tile/DungeonTileMap.h"
#include "../Tile/BgTileManager.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitMovement.h"
#include "../Control/PlayerController.h"

DungeonScene::DungeonScene()
{
	tileMap = new DungeonTileMap("TextData/TinyWood1.map");
	CAM->SetLeftBottom(tileMap->LeftBottom());
	CAM->SetRightTop(tileMap->RightTop());

	Observer::Get()->AddGetEvent("CallTileMap", bind(&DungeonScene::CallTileMap, this, placeholders::_1));
	
	testUnit = new Unit(new PlayerController, Vector2(80, 80));
	CAM->SetTarget(testUnit);
	testUnit->SetPos(24, 14);
	testUnit->SetDir(0, -1);


}

DungeonScene::~DungeonScene()
{
	delete tileMap;
	BgTileManager::Delete();

	delete testUnit;
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
		//로그를 제외한 어느 UI라도 열려 있으면 그쪽을 우선
		//UI매니저 필요
		//로그 매니저 필요

		//목록 중 wait 수치가 가장 낮은 놈을 탐색
		int turn = 1e9;
		turn = min(turn, testUnit->GetWait());


		testUnit->GetWait() -= turn;
		if (testUnit->GetWait() == 0) {
			testUnit->GetController()->SetCommand();
		}

		if (testUnit->IsActing()) {
			actState = ACTING;
		}

		//주인공 유닛 행동-이동
		//동료 행동
		//적 행동
		//동료 이동
		//적 이동

	}
		break;
	case DungeonScene::ACTING:
	{
		bool isActing = false;

		//행동 중인 유닛이 있는지 검사
		//후에 유닛 목록으로 교체
		isActing |= testUnit->IsActing();

		//스킬도 발동 중인지 검사

		if (!isActing)
			actState = TURN_END;
	}
		break;

	case DungeonScene::TURN_END:
	{
		//행동한 녀석들 일제히 대기 시간 증가 및 행동 유닛 목록 비우기
		for (auto unit : actionUnits) {

		}

		for (auto unit : movingUnits) {

		}

		actionUnits.clear();
		movingUnits.clear();
		actState = WAIT_COMMAND;
	}
		break;
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
	switch (actState)
	{
	case DungeonScene::ENTER_DUNGEON:
		//던전 인트로 장면
		break;
	case DungeonScene::WAIT_COMMAND:

		break;
	case DungeonScene::ACTING:
		break;
	case DungeonScene::FLOOR_MOVE:
		break;
	case DungeonScene::PLAYER_DEAD:
		break;
	default:
		break;
	}

	//로그
}