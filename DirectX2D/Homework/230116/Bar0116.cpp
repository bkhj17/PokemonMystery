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

	Rot().z = down;
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
	Control();

	UpdateWorld();
	collider->UpdateWorld();
}

void Bar0116::Render()
{
	__super::Render();
	collider->Render();
}

void Bar0116::Control()
{
	if (press) {
		interpolationValue += speed * DELTA;
		isSwing = interpolationValue < 1.0f;

	}
	else {
		interpolationValue -= speed * DELTA;
		isSwing = false;
	}

	interpolationValue = Clamp(0.0f, 1.0f, interpolationValue);
	localRotation.z = Lerp(down, up, interpolationValue);
}
