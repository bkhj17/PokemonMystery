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
		//���� Ż��
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

	//���� �� �÷��̾ �ƴ�
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

	//1�̸� next, -1�̸� post
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
		//���ֵ� �ൿ ��
		isActing |= UnitManager::Get()->IsActing();
		if (isActing) {
			actState = ACTING;
			break;
		}
		//���� UI�� �ִٸ� �� ���� �켱

		if (PokemonUIManager::Get()->IsActing())
			break;

		//�� ����
		UnitManager::Get()->RunPhase();
		//���ΰ� ���� �ൿ
		//���� �ൿ
		//�� �ൿ
		//�� �̵�
	}
		break;
	case DungeonScene::ACTING:
	{
		bool isActing = false;

		//�ൿ ���� ������ �ִ��� �˻�
		//�Ŀ� ���� ������� ��ü
		isActing |= UnitManager::Get()->IsActing();
		//��ų�� �ߵ� ������ �˻�
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
		//���� ����
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
		//���� ��Ʈ�� ���
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

	//�α�
}

void DungeonScene::UnitMoveEnd(void* unitptr)
{
	Unit* unit = (Unit*)unitptr;

	if (!unit)
		return;

	tileMap->PosToPoint(unit->GlobalPos());
}
