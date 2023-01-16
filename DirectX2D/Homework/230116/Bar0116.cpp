#include "Framework.h"
#include "Bar0116.h"

Bar0116::Bar0116(Vector2 size, bool isLeft)
	: Quad(size), isLeft(isLeft)
{
	wstring file = L"Textures/Pinball/";
	file += isLeft ? L"LeftBar.png" : L"RightBar.png";
	SetTexture(file);
	
	down = XM_PI / 6.0f * (isLeft ? -1.0f : 1.0f);
	up = down + (isLeft ? 1.0f : -1.0f) * XM_PI / 3.0f;

	Rotation().z = curAngle = down;
	SetPivot({ size.x * (isLeft ? -0.5f : 0.5f), 0.0f });

	collider = new RectCollider(size);
	collider->SetParent(this);
}

Bar0116::~Bar0116()
{
	delete collider;
}

void Bar0116::Update()
{
	if (press) {
		if ((isLeft && curAngle < up) || (!isLeft && curAngle > up)) {
			curAngle += (up - curAngle > 0.0f ? 1.0f : -1.0f) * speed * DELTA;
		}
		curAngle = Clamp(min(up, down), max(up, down), curAngle);
	}
	else {
		if ((isLeft && curAngle > down) || (!isLeft && curAngle < down)) {
			curAngle += (down - curAngle > 0.0f ? 1.0f : -1.0f) * speed * DELTA;
		}
		curAngle = Clamp(min(up, down), max(up, down), curAngle);
	}

	Rotation().z = curAngle;

	__super::Update();
	collider->UpdateWorld();
}

void Bar0116::Render()
{
	__super::Render();
	collider->Render();
}
