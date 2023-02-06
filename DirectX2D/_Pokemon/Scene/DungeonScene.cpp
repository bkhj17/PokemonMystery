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
		//�α׸� ������ ��� UI�� ���� ������ ������ �켱
		//UI�Ŵ��� �ʿ�
		//�α� �Ŵ��� �ʿ�

		//��� �� wait ��ġ�� ���� ���� ���� Ž��
		int turn = 1e9;
		turn = min(turn, testUnit->GetWait());


		testUnit->GetWait() -= turn;
		if (testUnit->GetWait() == 0) {
			testUnit->GetController()->SetCommand();
		}

		if (testUnit->IsActing()) {
			actState = ACTING;
		}

		//���ΰ� ���� �ൿ-�̵�
		//���� �ൿ
		//�� �ൿ
		//���� �̵�
		//�� �̵�

	}
		break;
	case DungeonScene::ACTING:
	{
		bool isActing = false;

		//�ൿ ���� ������ �ִ��� �˻�
		//�Ŀ� ���� ������� ��ü
		isActing |= testUnit->IsActing();

		//��ų�� �ߵ� ������ �˻�

		if (!isActing)
			actState = TURN_END;
	}
		break;

	case DungeonScene::TURN_END:
	{
		//�ൿ�� �༮�� ������ ��� �ð� ���� �� �ൿ ���� ��� ����
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
		//���� ��Ʈ�� ���
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

	//�α�
}