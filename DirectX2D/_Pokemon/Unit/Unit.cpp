#include "Framework.h"
#include "Unit.h"
#include "UnitMovement.h"
#include "../Control/Controller.h"
#include "../Tile/DungeonTileMap.h"
#include "UnitManager.h"
#include "../Item/PlayerInventory.h"
#include "../Skill/Skill.h"
#include "../Log/Log.h"

Unit::Unit(Controller* controller, Vector2 size)
	: DungeonObject(size)
{
	SetController(controller);

	data = new PokemonData;

	skills.resize(5); //기본 공격 + 기술 4개
	skills[0] = new Skill(0);
	for (int i = 1; i <= 4; i++) {
		skills[i] = new Skill();
	}

	downQuad = new Quad(Vector2(40.0f, 40.0f));
	downQuad->SetPixelShader(L"Transit.hlsl");
	downQuad->SetTexture(L"Textures/pokemon/Down.png");
	downQuad->SetParent(this);
	downQuad->Pos() = { -size.x * 0.5f, size.y * 0.5f };

}

Unit::~Unit()
{
	SAFE_DELETE(animObject);
	SAFE_DELETE(controller);
	delete data;

}

void Unit::Init()
{
	isActive = true;
	data->curHp = data->statusData.maxHp;
	if (controller)
		controller->Init();
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

	animObject->GetColorBuffer()->Get() = { 1.0f, 1.0f, 1.0f, data->curHp > 0 ? 1.0f : 0.5f };
	animObject->Render();

	if (downTime > 0) {
		downQuad->Render();
	}
}

void Unit::CreateClipData()
{
	SAFE_DELETE(animObject);
		
	animObject = new AnimObject();
	animObject->SetParent(this);

	animObject->SetPixelShader(L"Transit.hlsl");
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

	if (!isActive)
		return;

	if (animObject == nullptr)
		return;

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
	downQuad->UpdateWorld();
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

	if (postKey != key) {
		InitSkillData(level);
		CreateClipData();
	}
}

void Unit::SetLevelData(int level)
{
	SetData(data->key, level);
	//레벨 변화 시의 처리
	AddNewSkill(level);
}

void Unit::TurnEnd()
{
	wait = max(0, wait - 2);
	//상태이상 턴 감소
	downTime -= 2;

	controller->TurnEnd();
}

bool Unit::PickUpItem(ItemData* itemData)
{
	if (controller->GetTag() == "Player" || controller->GetTag() == "Friend") {
		if(PlayerInventory::Get()->InputItem(itemData)) {
			//아이템 획득 로그(인벤토리)
			return true;
		}
	}

	if (carryItem == nullptr) {
		carryItem = itemData;
		//아이템 획득 로그(지니다)
		return true;
	}
	
	//못 먹음
	return false;
}

void Unit::SetDown(int d)
{
	if (downTime == 0 && d > 0)
		LogManager::Get()->InsertLog(data->statusData.name + "의 공격력이 떨어졌다");
	
	downTime = d;
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
	if (!isActive)
		return;

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

bool Unit::IsUsableSkill(int index, bool log)
{
	//스킬이 사용 가능한지 확인
	if (!skills[index] || !skills[index]->IsValid())
		return false;

	if (skills[index]->GetCurPP() == 0) {
		if (log) {
			//사용 불가능 로그
		}
		return false;
	}

	return true;
}

bool Unit::UseSkill(int index)
{
	if (!IsUsableSkill(index, true))
		return false;
	//무한 사용은 pp가 -1이므로 그 경우에는 pp가 변하지 않는다
	if(skills[index]->GetCurPP() > 0)	
		skills[index]->GetCurPP()--;

	//행동에 예약 걸기
	curSkill = index;
	//스킬 종류에 따라 행동 종류 결정
	bool IsPhysics = skills[index]->IsPhysics();
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
	if (damage > 0) {
		dirCode = dirCode / 100 * 100 + DAMAGE;
		animObject->SetClip(dirCode);
	}
	int postHp = data->curHp;
	data->curHp = min(max(data->curHp - damage, 0), data->statusData.maxHp);

	string logText;
	if (postHp < data->curHp) {
		//HP가 늘었다 = 회복했다
		logText = data->statusData.name + "은/는 " + to_string(data->curHp - postHp) + "만큼 회복했다.";
	}
	else {
		logText = data->statusData.name + "은/는 " + to_string(data->curHp - postHp) + "의 데미지를 입었다.";
	}

	if (!logText.empty())
		LogManager::Get()->InsertLog(logText);

	if (data->curHp <= 0)
		Die();

	if (damage != 0 && controller->GetTag() == "Player")
		Observer::Get()->ExecuteEvent("UpdateStatusUI");
}

void Unit::Die()
{
	Observer::Get()->ExecuteParamEvent("UnitDie", reinterpret_cast<void*>(this));	
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
	//curSkill 발동
	skills[curSkill]->Activate(this);
	SetIdle();
}

void Unit::InitSkillData(int level)
{
	//
	vector<int> skillKies;
	UnitManager::Get()->GetInitSkills(data->key, level, skillKies);

	for (int i = 0; i < skillKies.size(); i++)
		SetSkill(i + 1, skillKies[i]);
}

void Unit::SetSkill(int slot, int skillIndex)
{
	//유효하지 않은 슬롯
	if (slot < 1 || slot > 4)
		return;
	
	skills[slot]->SetData(skillIndex);
}

void Unit::AddNewSkill(int level)
{
	//레벨 변화로 인해 스킬을 새로 익힌다
	vector<int> tmp;
	UnitManager::Get()->GetSkillDataKey(data->key, level, tmp);
	if (tmp.empty())
		return;

	int p = 0;
	for (int i = 1; i <= 4 && p < tmp.size(); i++) {
		if (!skills[i]->IsValid())
			SetSkill(i, tmp[p++]);
	}
}

bool Unit::IsActing()
{
	//스킬 시전 중인 경우, 혹은 데미지 입는 중인 경우
	//스킬 발동 중인건 스킬이 직접 알린다
	if (!isActive)
		return false;

	if (animTime > 0.0f && animTime < PHYSICS_TIME)
		return true;

	if (dirCode % 100 >= MOVING)
		return true;

	if (__super::IsActing())
		return true;

	return false;
}
