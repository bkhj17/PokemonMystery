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
	auto animData = UnitManager::Get()->GetAnimData(data->key);
	Vector2 cutSize = Texture::Add(animData->textureFile)->GetSize() / animData->texcoord;
	vector<Frame*> frames;
	for (int dir = 0; dir < 9; dir++) {
		if (dir == 4)
			continue;

		for (int action = 0; action <= DAMAGE; action++) {
			int clipCode = dir * 100 + action;

			vector<POINT> vp = animData->clipData[clipCode];
			for (POINT p : vp)
				frames.push_back(new Frame(animData->textureFile, cutSize.x * p.x, cutSize.y * p.y, cutSize.x, cutSize.y));
			
			auto clip = new Clip(frames, action <= MOVING);
			if (action > MOVING)
				clip->SetEvent(bind(action <= SKILL_SPECIAL ? &Unit::SkillActivate : &Unit::SetIdle, this));

			animObject->AddClip(clipCode, clip);
			
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
	if (dirCode % 100 >= SKILL_PHYSICS)
		return;

	int center = 4;
	dirCode = (4 + -animDirY * 3 + animDirX) * 100;
	//��ų ��� ���� ���

	//�̵� ���� ���
	if (movement->IsMoving())
		dirCode += MOVING;
	
	animObject->SetClip(dirCode);
}

bool Unit::IsCollide()
{
	Unit* check = UnitManager::Get()->GetPlayer();
	if (check != this 
		&& check->GetPoint().x == GetPoint().x
		&& check->GetPoint().y == GetPoint().y) {
		return true;
	}

	for (auto f : UnitManager::Get()->GetFriends()) {		
		check = f;
		if (check != this
			&& check->GetPoint().x == GetPoint().x
			&& check->GetPoint().y == GetPoint().y)
			return true;
	}

	for (auto e : UnitManager::Get()->GetEnemies()) {
		check = e;
		if (check != this
			&& check->GetPoint().x == GetPoint().x
			&& check->GetPoint().y == GetPoint().y)
			return true;
	}


	return false;
}

bool Unit::UseSkill(int index)
{
	//��ų�� ��� �������� Ȯ���ؾ� ��
	if (false) {
		//��� �Ұ��� �α�
		return false;
	}
	
	curSkill = index;
	//��ų ������ ���� �ൿ ���� ����
	bool IsPhysics = true;
	int action = IsPhysics ? SKILL_PHYSICS : SKILL_SPECIAL;

	dirCode = dirCode / 100 * 100 + action;
	animObject->SetClip(dirCode);
	return true;
}

void Unit::SetIdle()
{
	this->dirCode = 0;
	SetAction();
}

void Unit::SkillActivate()
{
	//curSkill�� �����ϴ� effectObject�� ����




	SetIdle();
}

bool Unit::IsActing()
{
	//��ų ���� ���� ���, Ȥ�� ������ �Դ� ���� ���
	//��ų �ߵ� ���ΰ� ��ų�� ���� �˸���
	if (dirCode % 100 > IsMoving())
		return true;

	if (__super::IsActing())
		return true;

	return false;
}

bool Unit::Test()
{
	if (IsActing())
		return false;

	dirCode = dirCode / 100 * 100 + SKILL_PHYSICS;
	animObject->SetClip(dirCode);

	return true;
}
