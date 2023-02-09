#include "Framework.h"
#include "DungeonScene.h"
#include "../Tile/DungeonTileMap.h"
#include "../Tile/BgTileManager.h"
#include "../Unit/Unit.h"
#include "../Unit/UnitMovement.h"
#include "../Control/PlayerController.h"
#include "../Data/ItemDataManager.h"
#include "../Data/DungeonDataManager.h"
#include "../UI/PokemonUIManager.h"
#include "../Unit/UnitManager.h"

DungeonScene::DungeonScene()
{
	ItemDataManager::Get();
	DungeonDataManager::Get();
	PokemonUIManager::Get();
	UnitManager::Get()->Init();

	tileMap = new DungeonTileMap();
	Observer::Get()->AddGetEvent("CallTileMap", bind(&DungeonScene::CallTileMap, this, placeholders::_1));
	
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
	ItemDataManager::Delete();
	UnitManager::Delete();
}

void DungeonScene::InitFloor(string name, int floor)
{
	if (floor == 0) {
		//던전 탈출
		return;
	}
	tileMap->Init(name, floor);
	Observer::Get()->ExecuteEvent("UpdateStatusUI");

	auto player = UnitManager::Get()->GetPlayer();
	player->SetDir(0, -1);
	player->SetPoint(tileMap->GetPlayerStartPoint());
	CAM->SetTarget(UnitManager::Get()->GetPlayer());

	UnitManager::Get()->GetEnemies()[0]->SetPoint(8, 12);



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
		bool isActing = false;
		//유닛들 행동 중
		isActing |= UnitManager::Get()->IsActing();
		if (isActing) {
			actState = ACTING;
			break;
		}

		//열린 UI가 있다면 그 쪽을 우선

		PokemonUIManager::Get()->Update();
		
		//턴 진행
		UnitManager::Get()->RunPhase();

		//주인공 유닛 행동
		//동료 행동
		//적 행동
		//적 이동
	}
		break;
	case DungeonScene::ACTING:
	{
		bool isActing = false;

		//행동 중인 유닛이 있는지 검사
		//후에 유닛 목록으로 교체
		
		isActing |= UnitManager::Get()->IsActing();


		//스킬도 발동 중인지 검사
		

		if (!isActing)
			actState = TURN_END;
	}
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
		//게임 오버
		break;
	default:
		break;
	}

	UnitManager::Get()->Update();
}

void DungeonScene::Render()
{
	tileMap->Render();
	UnitManager::Get()->Render();
}

void DungeonScene::PostRender()
{
	PokemonUIManager::Get()->PostRender();


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

void DungeonScene::UnitMoveEnd(void* unitptr)
{
	Unit* unit = (Unit*)unitptr;

	if (!unit)
		return;

	tileMap->PosToPoint(unit->GlobalPos());


}
