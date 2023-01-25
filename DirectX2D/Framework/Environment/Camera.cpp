#include "Framework.h"

Camera::Camera()
{
	tag = "Camera";

	viewBuffer = new MatrixBuffer();
	viewBuffer->SetVS(1);
}

Camera::~Camera()
{
	delete viewBuffer;
}

void Camera::Update()
{
	(target != nullptr) ? FollowMode() : FreeMode();

	SetView();
}

void Camera::SetView()
{
	UpdateWorld();

	view = XMMatrixInverse(nullptr, world);

	viewBuffer->Set(view);
	viewBuffer->SetVS(1);
}

void Camera::FreeMode()
{
	if (KEY_PRESS(VK_RBUTTON)) {
		if (KEY_PRESS('W'))
			Pos().y += speed * DELTA;
		if (KEY_PRESS('S'))
			Pos().y -= speed * DELTA;
		if (KEY_PRESS('D'))
			Pos().x += speed * DELTA;
		if (KEY_PRESS('A'))
			Pos().x -= speed * DELTA;
	}
}

void Camera::FollowMode()
{
	Pos() = target->GlobalPos() - Vector2(CENTER_X, CENTER_Y);
}
