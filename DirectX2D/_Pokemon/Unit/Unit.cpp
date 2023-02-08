#include "Framework.h"
#include "Unit.h"
#include "UnitMovement.h"
#include "../Control/Controller.h"
#include "../Tile/DungeonTileMap.h"
#include "UnitManager.h"

Unit::Unit(Controller* controller, Vector2 size)
	: DungeonObject(size)
{
	CreateClipData();

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

	__super::SetRender();
	animObject->Render();
	collider->Render();
}

void Unit::CreateClipData()
{
	animObject = new AnimObject();
	animObject->SetParent(this);

	//애니메이션 클립 설정
	wstring textureFile = L"Textures/pokemon/이상해씨.png";
	Vector2 cutSize = Texture::Add(textureFile)->GetSize() / Vector2(10, 8);
	vector<Frame*> frames;

	//LeftUp : 0
	int dirCode = 0;
	//IDLE
	dirCode = 0;
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 3, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100, new Clip(frames));
	frames.clear();
	//MOVING
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 3, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 3, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 3, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100 + 1, new Clip(frames));
	frames.clear();

	//Up : 1
	dirCode = 1;
	//IDLE
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 4, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100, new Clip(frames));
	frames.clear();
	//MOVING
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 4, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 4, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 4, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100+1, new Clip(frames));
	frames.clear();

	//RightUp : 2
	dirCode = 2;
	//IDLE
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 5, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100, new Clip(frames));
	frames.clear();
	//MOVING
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 5, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 5, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 5, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100 + 1, new Clip(frames));
	frames.clear();

	//Left : 3
	dirCode = 3;
	//IDLE
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 2, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100, new Clip(frames));
	frames.clear();
	//MOVING
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 2, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100 + 1, new Clip(frames));
	frames.clear();

	//Right : 5
	dirCode = 5;
	//IDLE
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 6, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100, new Clip(frames));
	frames.clear();
	//MOVING
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 6, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 6, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 6, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100 + 1, new Clip(frames));
	frames.clear();
	
	//LeftDown : 6
	dirCode = 6;
	//IDLE
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 1, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100, new Clip(frames));
	frames.clear();
	//MOVING
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 1, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 1, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 1, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100 + 1, new Clip(frames));
	frames.clear();

	//Down : 7, default
	dirCode = 7;
	//IDLE
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 0, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100, new Clip(frames));
	frames.clear();
	//MOVING
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 0, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100 + 1, new Clip(frames));
	frames.clear();
	//DAMAGE	
	
	//RightDown : 8
	dirCode = 8;
	//IDLE
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 7, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100, new Clip(frames));
	frames.clear();
	//MOVING
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 7, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 7, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 7, cutSize.x, cutSize.y));
	animObject->AddClip(dirCode * 100 + 1, new Clip(frames));
	frames.clear();
}

void Unit::UpdateWorld()
{
	__super::UpdateWorld();
	animObject->UpdateWorld();
}

void Unit::SetController(Controller* controller)
{
	this->controller = controller;
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
	UnitManager::Get()->GetPokemonData(key, level, data);
}

void Unit::SetLevelData(int level)
{
	SetData(data->key, level);
}

void Unit::TurnEnd()
{
	wait = max(0, wait - 2);
	//상태이상도 


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
		dirCode += 1;
	


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

	//스킬 시전 중인 경우
	//스킬 발동 중인건 스킬이 직접 알린다

	return false;
}
