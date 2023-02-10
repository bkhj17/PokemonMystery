#include "Framework.h"
#include "Unit.h"
#include "UnitMovement.h"
#include "../Control/Controller.h"
#include "../Tile/DungeonTileMap.h"
#include "UnitManager.h"
#include "../Item/PlayerInventory.h"

Unit::Unit(Controller* controller, Vector2 size)
	: DungeonObject(size)
{
	//CreateClipData();

	SetController(controller);

	data = new PokemonData;
}

Unit::~Unit()
{
	delete animObject;
	delete controller;
	delete data;
}

void Unit::Update()
{
	if (!isActive)
		return;

	MovementUpdate();
	
	SetAction();
	animObject->Update();
	UpdateWorld();
}

void Unit::Render()
{
	if (!isActive)
		return;
	__super::Render();
	animObject->Render();
}

void Unit::CreateClipData()
{
	SAFE_DELETE(animObject);
		
	animObject = new AnimObject();
	animObject->SetParent(this);

	//�ִϸ��̼� Ŭ�� ����
	//������ȭ �����ϸ� ���ڴٸ�
	wstring textureFile = L"Textures/pokemon/�̻��ؾ�.png";
	map<pair<int, int>, vector<POINT>> clipMap;
	//LeftUp : 0
	int dirCode = 0;
	//0 IDLE
	//1 MOVING
	clipMap[{1, dirCode * 100 + IDLE}].push_back({ 0, 3 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 0, 3 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 1, 3 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 2, 3 });

	//Up : 1
	dirCode = 1;
	clipMap[{1, dirCode * 100 + IDLE}].push_back({ 0, 4 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 0, 4 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 1, 4 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 2, 4 });

	//RightUp : 2
	dirCode = 2;
	clipMap[{1, dirCode * 100 + IDLE}].push_back({ 0, 5 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 0, 5 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 1, 5 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 2, 5 });

	//Left : 3
	dirCode = 3;
	clipMap[{1, dirCode * 100 + IDLE}].push_back({ 0, 2 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 0, 2 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 1, 2 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 2, 2 });

	//4�� �߾� => ��꿡 ���� �ʾ� ���� �ʴ´�

	//Right : 5
	dirCode = 5;
	clipMap[{1, dirCode * 100 + IDLE}].push_back({ 0, 6 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 0, 6 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 1, 6 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 2, 6 });

	//LeftDown : 6
	dirCode = 6;
	clipMap[{1, dirCode * 100 + IDLE}].push_back({ 0, 1 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 0, 1 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 1, 1 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 2, 1 });

	//Down : 7, default
	dirCode = 7;
	clipMap[{1, dirCode * 100 + IDLE}].push_back({ 0, 0 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 0, 0 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 1, 0 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 2, 0 });

	//RightDown : 8
	dirCode = 8;
	clipMap[{1, dirCode * 100 + IDLE}].push_back({ 0, 7 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 0, 7 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 1, 7 });
	clipMap[{1, dirCode * 100 + MOVING}].push_back({ 2, 7 });


	Vector2 cutSize = Texture::Add(textureFile)->GetSize() / Vector2(10, 8);
	vector<Frame*> frames;
	int pokemonKey = 1;
	for (int dir = 0; dir < 9; dir++) {
		if (dir == 4)
			continue;

		for (int action = 0; action <= 1; action++) {
			int clipCode = dir * 100 + action;
			for (POINT p : clipMap[{pokemonKey, clipCode}])
				frames.push_back(new Frame(textureFile, cutSize.x * p.x, cutSize.y * p.y, cutSize.x, cutSize.y));
			animObject->AddClip(clipCode, new Clip(frames));
			frames.clear();
		}
	}
}

void Unit::UpdateWorld()
{
	__super::UpdateWorld();
	animObject->UpdateWorld();
}

void Unit::SetController(Controller* controller)
{
	this->controller = controller;
	if(controller != nullptr)
		controller->SetUnit(this);
}

void Unit::SetDir(int x, int y)
{
	animDirX = x;
	animDirY = y;
	SetAction();
}

void Unit::SetData(int key, int level)
{
	int postKey = data->key;
		UnitManager::Get()->GetPokemonData(key, level, data);
	
	if (postKey != key)
		CreateClipData();
}

void Unit::SetLevelData(int level)
{
	SetData(data->key, level);
}

void Unit::TurnEnd()
{
	wait = max(0, wait - 2);
	//�����̻� �� ����
}

bool Unit::PickUpItem(ItemData* itemData)
{
	if (controller->GetTag() == "Player" 
		|| controller->GetTag() == "Friend") {
		if(PlayerInventory::Get()->InputItem(itemData)) {

			//������ ȹ�� �α�
			return true;
		}
	}

	if (carryItem == nullptr) {
		carryItem = itemData;
		//������ ȹ�� �α�
		return true;
	}
	
	//�� ����
	return false;
}

void Unit::SetAction()
{
	/*
	dir
	  -1
	-1 0 1
	   1
	dirCode
	0 1 2
	3 4 5
	6 7 8
	*/
	int center = 4;

	int dirCode = (4 + -animDirY * 3 + animDirX) * 100;
	if (movement->IsMoving())
		dirCode += MOVING;
	


	animObject->SetClip(dirCode);
}

bool Unit::IsCollide()
{
	return false;
}

bool Unit::IsActing()
{
	if (__super::IsActing())
		return true;

	//��ų ���� ���� ���
	//��ų �ߵ� ���ΰ� ��ų�� ���� �˸���

	return false;
}
