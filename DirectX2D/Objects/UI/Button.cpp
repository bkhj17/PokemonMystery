#include "Framework.h"
#include "Button.h"

Button::Button(wstring textureFile)
	: Quad(textureFile)
{
	event = nullptr;
	paramEvent = nullptr;
	object = nullptr;
	collider = new RectCollider(size);
	collider->SetParent(this);
}

Button::Button(Vector2 size)
	: Quad(size)
{
	event = nullptr;
	paramEvent = nullptr;
	object = nullptr;

	collider = new RectCollider(size);
	collider->SetParent(this);
}

Button::~Button()
{
	delete collider;
}

void Button::Update()
{
	if (!isActive)
		return;

	if (collider->IsPointCollision(mousePos)) {
		if (KEY_DOWN(VK_LBUTTON))
			isDownCheck = true;
		
		if (KEY_PRESS(VK_LBUTTON))
			state = DOWN;
		else
			state = OVER;

		if (isDownCheck && KEY_UP(VK_LBUTTON)) {
			ExecuteEvent();
			isDownCheck = false;
		}
	}
	else {
		state = NONE;
		if(KEY_UP(VK_LBUTTON))
			isDownCheck = false;
	}

	switch (state)
	{
	case Button::NONE:
		colorBuffer->Get() = NONE_COLOR;
		break;
	case Button::DOWN:
		colorBuffer->Get() = DOWN_COLOR;
		break;
	case Button::OVER:
		colorBuffer->Get() = OVER_COLOR;
		break;
	default:
		break;
	}

	UpdateWorld();
}

void Button::UpdateWorld()
{
	__super::UpdateWorld();

	collider->UpdateWorld();
}

void Button::Render()
{
	if (!isActive)
		return;

	colorBuffer->SetPS(0);

	__super::Render();

	//collider->Render();
}

void Button::ExecuteEvent()
{
	if (event) 
		event();
	if (paramEvent && object != nullptr)
		paramEvent(object);
}
