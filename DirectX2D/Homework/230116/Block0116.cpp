#include "Framework.h"
#include "Block0116.h"

Block0116::Block0116(Vector2 size)
	: Quad(size)
{
	SetTexture(L"Textures/Shooting/rectEnemy.png");

	collider = new RectCollider(size);
	collider->SetParent(this);
}

Block0116::~Block0116()
{
	delete collider;
}

void Block0116::Update()
{
	FallDown();

	collider->UpdateWorld();
	UpdateWorld();
}

void Block0116::FallDown()
{
	fallDownTime -= DELTA;
	if (fallDownTime < 0.0f) {
		fallDownTime = FALL_DOWN_RATE;
		Pos().y -= size.y;
	}
}

void Block0116::Break()
{
	isActive = false;
	breakFunc();
}
