#include "Framework.h"
#include "DungeonScene.h"

DungeonScene::DungeonScene()
{
	EffectManager::Get();
	EffectObjectManager::Get();
	UnitManager::Get()->Init();
	ItemObjectManager::Get();
	BgTileManager::Get();

	tileMap = new DungeonTileMap();
	Observer::Get()->AddGetEvent("CallTileMap", bind(&DungeonScene::CallTileMap, this, placeholders::_1));
	
	Observer::Get()->AddEvent("ExitDungeon", bind(&DungeonScene::SetFloorMove, this, EXIT));
	Observer::Get()->AddEvent("ShiftNextFloor", bind(&DungeonScene::SetFloorMove, this, NEXT));
	Observer::Get()->AddEvent("ShiftPostFloor", bind(&DungeonScene::SetFloorMove, this, POST));
	Observer::Get()->AddParamEvent("UnitDie", bind(&DungeonScene::PlayerDie, this, placeholders::_1));


	gameOver = new Quad(L"Textures/Shooting/GameOver.png");
	gameOver->Pos() = { CENTER_X, CENTER_Y };
	gameOver->UpdateWorld();

	Observer::Get()->AddParamEvent("MoveEnd", [](void* ptr) {
		Unit* unit = (Unit*)ptr;
		if (unit == nullptr)
			return;
		if (unit->GetController()->GetTag() != "Enemy")
			unit->Damage(-1, false);
	});
}

DungeonScene::~DungeonScene()
{
	delete tileMap;
	UnitManager::Get()->ClearEnemy();

	ItemObjectManager::Delete();
	BgTileManager::Delete();
	EffectObjectManager::Delete();
	EffectManager::Delete();

	delete gameOver;
}

void DungeonScene::Start()
{
	InitFloor("TinyWood", -1);
	CAM->SetTarget(UnitManager::Get()->GetPlayer());
}

void DungeonScene::InitFloor(string name, int floor)
{
	if (floor == 0) {
		//던전 탈출
		EscapeDungeon();
		return;
	}
	tileMap->Init(name, floor);
	Observer::Get()->ExecuteEvent("UpdateStatusUI");

	auto player = UnitManager::Get()->GetPlayer();
	player->SetDir(0, -1);
	player->SetDown(0);
	player->SetPoint(tileMap->GetPlayerStartPoint());

	UnitManager::Get()->ClearEnemy();
	UnitManager::Get()->InitEnemy();

	Observer::Get()->ExecuteEvent("ClearMiniMap");
	Observer::Get()->ExecuteParamEvent("MoveEnd", player);

	floorMove = 0;
	actState = ENTER_DUNGEON;
}

void DungeonScene::PlayerDie(void* unitPtr)
{
	Unit* unit = reinterpret_cast<Unit*>(unitPtr);
	if (!unit)
		return;

	//죽은 게 플레이어가 아님
	if (unit->GetController()->GetTag() != "Player")
		return;

	actState = PLAYER_DEAD;
}

void DungeonScene::EscapeDungeon()
{
	ClearData::Get()->SetDungeonData(tileMap->GetFloorData());
	ClearData::Get()->SetPlayerData(UnitManager::Get()->GetPlayer()->GetData());
	SceneManager::Get()->ChangeScene("Clear");
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

		if (PokemonUIManager::Get()->IsActing())
			break;

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
		isActing |= EffectObjectManager::Get()->IsActing();

		if (!isActing)
			actState = TURN_END;
	}
		break;
	case DungeonScene::TURN_END:
	{
		actState = WAIT_COMMAND;

		if (Random(0, 10) == 0)
			UnitManager::Get()->InitEnemy();
	}
		break;
	case DungeonScene::FLOOR_MOVE:
		break;
	case DungeonScene::PLAYER_DEAD:
		//게임 오버
		break;
	default:
		break;
	}

	PokemonUIManager::Get()->Update();
	UnitManager::Get()->Update();
	ItemObjectManager::Get()->Update();
	EffectObjectManager::Get()->Update();
	EffectManager::Get()->Update();
}

void DungeonScene::Render()
{
	tileMap->Render();
	ItemObjectManager::Get()->Render();
	UnitManager::Get()->Render();
	EffectObjectManager::Get()->Render();
	EffectManager::Get()->Render();
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
		gameOver->PostRender();
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
