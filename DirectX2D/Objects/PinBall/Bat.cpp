#include "Framework.h"
#include "Bat.h"

Bat::Bat(bool isRight)
	: Quad(L"Textures/Pinball/Bat.png"), isRight(isRight)
{
	collider = new RectCollider(texture->GetSize());
	collider->SetParent(this);

	SetPivot({ -size.x * 0.5f, 0.0f});
	if (isRight) {
		localPosition = { 750, 140 };
		localRotation.z = RIGHT_MIN_ANGLE;
	}
	else {
		localPosition = { 140, 140 };
		localRotation.z = LEFT_MIN_ANGLE;
	}
	Update();
}

Bat::~Bat()
{
	delete collider;
}

void Bat::Update()
{
	Control();

	UpdateWorld();
	collider->UpdateWorld();
}

void Bat::Render()
{
	__super::Render();
	collider->Render();
}

void Bat::Control()
{
	if (isRight && KEY_PRESS(VK_RBUTTON)
		|| !isRight && KEY_PRESS(VK_LBUTTON))
	{
		interpolationValue += speed * DELTA;
		isSwing = interpolationValue < 1.0f;
	}
	else {
		interpolationValue -= speed * DELTA;
		isSwing = false;
	}

	interpolationValue = Clamp(0.0f, 1.0f, interpolationValue);
	if (isRight) {
		localRotation.z = Lerp(RIGHT_MIN_ANGLE, RIGHT_MAX_ANGLE, interpolationValue);
	}
	else {
		localRotation.z = Lerp(LEFT_MIN_ANGLE, LEFT_MAX_ANGLE, interpolationValue);
	}


}
