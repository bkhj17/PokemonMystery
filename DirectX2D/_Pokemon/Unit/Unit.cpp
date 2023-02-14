#include "Framework.h"
#include "Unit.h"
#include "UnitMovement.h"
#include "../Control/Controller.h"
#include "../Tile/DungeonTileMap.h"
#include "UnitManager.h"
#include "../Item/PlayerInventory.h"
#include "../Skill/Skill.h"

Unit::Unit(Controller* controller, Vector2 size)
	: DungeonObject(size)
{
	//CreateClipData();

	SetController(controller);

	data = new PokemonData;

	skills.resize(5); //기본 공격 + 기술 4개
	skills[0] = new Skill(0);
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

	//애니메이션 클립 설정
	//데이터화 가능하면 좋겠다만
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

	if (animOffset.x != 0 || animOffset.y != 0) {
		animTime += DELTA;
		if (animTime < PHYSICS_TIME * 0.5f) {
			animObject->Pos() = Lerp(Vector2(), animOffset, animTime / PHYSICS_TIME * 2.0f);
		}
		else if(animTime >= PHYSICS_TIME) {
			animOffset = { 0, 0 };
			animObject->Pos() = Vector2();
		}
		else {
			animObject->Pos() = Lerp(animOffset, Vector2(), (animTime / PHYSICS_TIME - 0.5f) * 2.0f);
		}
	}
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
	//상태이상 턴 감소
}

bool Unit::PickUpItem(ItemData* itemData)
{
	if (controller->GetTag() == "Player" 
		|| controller->GetTag() == "Friend") {
		if(PlayerInventory::Get()->InputItem(itemData)) {
			//아이템 획득 로그
			return true;
		}
	}

	if (carryItem == nullptr) {
		carryItem = itemData;
		//아이템 획득 로그
		return true;
	}
	
	//못 먹음
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
	//스킬 사용 중일 경우

	//이동 중일 경우
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
		if (!check->Active())
			continue;
		if (check != this
			&& check->GetPoint().x == GetPoint().x
			&& check->GetPoint().y == GetPoint().y)
			return true;
	}

	for (auto e : UnitManager::Get()->GetEnemies()) {
		check = e;
		if (!check->Active())
			continue;
		if (check != this
			&& check->GetPoint().x == GetPoint().x
			&& check->GetPoint().y == GetPoint().y)
			return true;
	}


	return false;
}

bool Unit::UseSkill(int index)
{
	//스킬이 사용 가능한지 확인해야 함
	if (skills[index]->GetCurPP() == 0) {
		//사용 불가능 로그
		return false;
	}
	
	if (skills[index]->GetCurPP() > 0)
		skills[index]->GetCurPP()--;

	curSkill = index;
	//스킬 종류에 따라 행동 종류 결정
	bool IsPhysics = true;
	int action = IsPhysics ? SKILL_PHYSICS : SKILL_SPECIAL;

	dirCode = dirCode / 100 * 100 + action;
	animObject->SetClip(dirCode);
	if (IsPhysics) {
		animOffset = { animDirX * size.x, animDirY * size.y };
		animTime = 0.0f;
	}

	return true;
}

void Unit::Damage(int damage)
{
	dirCode = dirCode / 100 * 100 + DAMAGE;
	animObject->SetClip(dirCode);
	if ((data->curHp -= damage) <= 0) {
		Die();
	}
}

void Unit::Die()
{

}

void Unit::SetIdle()
{
	this->dirCode = 0;
	SetAction();

	if (data->curHp <= 0)
		isActive = false;
}

void Unit::SkillActivate()
{
	//curSkill에 대응하는 effectObject를 생성
	//skill에서 하자
	//시전자 위치, 
	//스킬 코드만 들고 있을까? 
	skills[curSkill]->Activate(this);
	SetIdle();
}

void Unit::SetSkillData(int level)
{
}

void Unit::AddNewSkill(int level)
{
}

bool Unit::IsActing()
{
	//스킬 시전 중인 경우, 혹은 데미지 입는 중인 경우
	//스킬 발동 중인건 스킬이 직접 알린다
	if (!isActive)
		return false;


	if (animTime > 0.0f && animTime < PHYSICS_TIME)
		return true;

	if (dirCode % 100 > MOVING)
		return true;


	if (__super::IsActing())
		return true;

	return false;
}
