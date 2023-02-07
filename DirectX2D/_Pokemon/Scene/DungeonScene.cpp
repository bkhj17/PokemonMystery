#include "Framework.h"
#include "DungeonScene.h"
#include "../Tile/DungeonTileMap.h"
#include "../Tile/BgTileManager.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitMovement.h"
#include "../Control/PlayerController.h"
#include "../Data/DungeonDataManager.h"
#include "../UI/PokemonUIManager.h"

DungeonScene::DungeonScene()
{
	DungeonDataManager::Get();
	PokemonUIManager::Get();

	tileMap = new DungeonTileMap();
	Observer::Get()->AddGetEvent("CallTileMap", bind(&DungeonScene::CallTileMap, this, placeholders::_1));
	
	testUnit = new Unit(new PlayerController, Vector2(80, 80));
	CAM->SetTarget(testUnit);

	Observer::Get()->AddEvent("ExitDungeon", bind(&DungeonScene::SetFloorMove, this, EXIT));
	Observer::Get()->AddEvent("ShiftNextFloor", bind(&DungeonScene::SetFloorMove, this, NEXT));
	Observer::Get()->AddEvent("ShiftPostFloor", bind(&DungeonScene::SetFloorMove, this, POST));

	InitFloor("TinyWood", -1);
}

DungeonScene::~DungeonScene()
{
	delete tileMap;
	BgTileManager::Delete();
	PokemonUIManager::Delete();

	delete testUnit;
}

void DungeonScene::InitFloor(string name, int floor)
{
	if (floor == 0) {
		//던전 탈출
		return;
	}
	tileMap->Init(name, floor);

	CAM->SetLeftBottom(tileMap->LeftBottom());
	CAM->SetRightTop(tileMap->RightTop());

	testUnit->SetDir(0, -1);
	testUnit->SetPos(24, 14);

	floorMove = 0;
	actState = ENTER_DUNGEON;
}

void DungeonScene::ShiftNextFloor()
{
	if (floorMove == NONE)
		return;

	if (floorMove == EXIT)
		InitFloor("", 0);

	//1이면 next, -1이면 post
	auto data = tileMap->GetFloorData();
	InitFloor(data->dungeonKey, floorMove > 0 ? data->nextFloor : data->postFloor);
}

void DungeonScene::Update()
{
	ShiftNextFloor();

	switch (actState)
	{
	case DungeonScene::ENTER_DUNGEON:
		actState = WAIT_COMMAND;
		break;
	case DungeonScene::WAIT_COMMAND:
	{
		//플레이어의 입력을 대기
		PokemonUIManager::Get()->Update();

		int turn = 1e9;
		turn = min(turn, testUnit->GetWait());

		testUnit->GetWait() -= turn;
		if (testUnit->GetWait() == 0) {
			testUnit->GetController()->SetCommand();
		}

		bool isActing = false;
		isActing |= testUnit->IsActing();

		if (KEY_DOWN(VK_SPACE)) {
			InitFloor("TinyWood", -2);
			actState = ENTER_DUNGEON;
			break;
		}

		if (isActing)
			actState = PLAYER_ACTING;

		//주인공 유닛 행동
		//동료 행동
		//적 행동
		//적 이동
	}
		break;
	case DungeonScene::PLAYER_ACTING:
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
	case DungeonScene::PARTNER_COMMAND:
		break;
	case DungeonScene::PARTNER_ACTING:
		break;
	case DungeonScene::ELSE_COMMAND:
		break;
	case DungeonScene::ELSE_ACTING:



		break;
	case DungeonScene::TURN_END:
	{
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
	case DungeonScene::PLAYER_ACTING:
		break;
	case DungeonScene::PARTNER_ACTING:
		break;
	case DungeonScene::ELSE_ACTING:
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

void DungeonScene::UnitMoveEnd(void* unitptr)
{
	Unit* unit = (Unit*)unitptr;

	if (!unit)
		return;

	tileMap->PosToPoint(unit->GlobalPos());


}
