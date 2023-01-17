#include "Framework.h"
#include "Block0116.h"

Block0116::Block0116(Vector2 size)
	: size(size)
{
	LoadClips();

	curClip = clips[IDLE];
	curClip->Play();
	Vector2 textureSize = curClip->GetCurFrame()->GetSize();
	localScale = { size.x / curClip->GetCurFrame()->GetSize().x, size.y / curClip->GetCurFrame()->GetSize().y };
	
	collider = new RectCollider(curClip->GetCurFrame()->GetSize());
	collider->SetParent(this);
}

Block0116::~Block0116()
{
	delete collider;
}

void Block0116::Update()
{
	if (!isActive)
		return;

	if(curAction == IDLE)
		FallDown();

	if (curAction == HIT && !curClip->IsPlay())
		ChangeAction(IDLE);

	if (curAction == DEAD && !curClip->IsPlay()) {
		Break();
	}

	curClip->Update();


	collider->UpdateWorld();
	UpdateWorld();
}

void Block0116::Render()
{
	if (!isActive)
		return;

	__super::Render();
	collider->Render();
}

void Block0116::FallDown()
{
	Pos().y -= speed * DELTA;
}

void Block0116::Spawn()
{
	isActive = true;
	hitPoint = MAX_HIT_POINT;
	ChangeAction(IDLE);
}

void Block0116::Break()
{
	isActive = false;
	breakFunc();
}

void Block0116::Hit()
{
	ChangeAction(HIT);
}

void Block0116::Damaged()
{
	if (--hitPoint <= 0)
		ChangeAction(DEAD);
}

void Block0116::ChangeAction(ActionType action)
{
	if (action == curAction)
		return;
	curAction = action;
	curClip = clips[(int)curAction];
	curClip->Play();
}

void Block0116::LoadClips()
{
	string path = "Textures/Ninja/";
	LoadClip(IDLE, path, "Idle.xml", true, 1.0f);
	LoadClip(HIT, path, "Attack.xml", false, 3.0f);
	LoadClip(DEAD, path, "Dead.xml", false, 1.0f);

	clips[HIT]->events[8] = bind(&Block0116::Damaged, this);
}
