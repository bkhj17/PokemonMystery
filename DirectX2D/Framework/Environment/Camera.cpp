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

void Camera::RenderUI()
{
	__super::RenderUI();

	string temp = tag + "_speed";
	ImGui::InputFloat(temp.c_str(), &speed);
}

void Camera::SetView()
{
	UpdateWorld();

	view = XMMatrixInverse(nullptr, world);

	viewBuffer->Set(view);
	viewBuffer->SetVS(1);
}

Vector2 Camera::ScreenToWorld(Vector2 pos)
{
	return pos * world;
}

Vector2 Camera::WorldToScreen(Vector2 pos)
{
	return pos * view;
}

bool Camera::ContainFrustum(Vector2 pos, Vector2 size)
{
	float left = pos.x - size.x * 0.5f;
	float right = pos.x + size.x * 0.5f;
	float top = pos.y + size.y * 0.5f;
	float bottom = pos.y - size.y * 0.5f;

	if (Pos().x < right && Pos().x + deviceSize.x > left) {
		if (Pos().y < top && Pos().y + deviceSize.y > bottom)
			return true;
	}

	return false;
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
	FixPosition(localPosition);
}

void Camera::FollowMode()
{
	Vector2 targetPos = target->GlobalPos() - targetOffset;
	FixPosition(targetPos);

	localPosition = Lerp(localPosition, targetPos, speed * DELTA);
}

void Camera::FixPosition(Vector2& position)
{
	position.x = max(position.x, leftBottom.x);
	position.y = max(position.y, leftBottom.y);
	position.x = min(position.x, rightTop.x - deviceSize.x);
	position.y = min(position.y, rightTop.y - deviceSize.y);
}
