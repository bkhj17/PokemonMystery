#include "Framework.h"
#include "Scene0119.h"
#include "Robot0119.h"

Scene0119::Scene0119()
{
	robots.resize(5);
	int cnt = 0;
	for (auto& robot : robots) {
		robot = new Robot0119;
		robot->Pos() = { CENTER_X - 400.0f + 120.0f * cnt++, CENTER_Y };
		robot->Update();
	}

	intValueBuffer = new IntValueBuffer();
	intValueBuffer->Get()[0] = 5;
	intValueBuffer->Get()[1] = 0;

	colorBuffer = new ColorBuffer();

	dragCollier = new RectCollider({1.0f, 1.0f});

}

Scene0119::~Scene0119()
{
	delete intValueBuffer;
	delete colorBuffer;

	for (auto robot : robots)
		delete robot;
	
	delete dragCollier;
}

void Scene0119::Update()
{
	if (KEY_DOWN(VK_LBUTTON)) {
		clickedPos = mousePos;
		for (auto robot : robots) {
			if (!robot->Active() || !robot->IsSelected())
				continue;

			if (robot->GetCollider()->IsPointCollision(mousePos)) {
				clickSelected = true;
				break;
			}
		}
	}

	if (clickSelected) {
		clickedPos = mousePos;
		MoveRobots();
	}
	else {
		DragOn();
	}

	for (auto robot : robots) {
		if (!robot->Active())
			continue;
		robot->Update();
	}
	
	if (KEY_UP(VK_LBUTTON)) {
		clickSelected = false;
	}

	//바로 직전의 마우스 위치
	postPos = mousePos;
}

void Scene0119::Render()
{
	colorBuffer->SetPS(0);


	for (auto robot : robots) {
		intValueBuffer->Get()[1] = robot->IsSelected() ? 1 : 0;
		intValueBuffer->SetPS(1);
		robot->Render();
	}

	dragCollier->Render();
}

void Scene0119::PostRender()
{
}

void Scene0119::DragOn()
{
	if (KEY_PRESS(VK_LBUTTON)) {
		dragCollier->SetActive(true);
		dragCollier->Pos() = (clickedPos + mousePos) * 0.5f;
		dragCollier->Scale() = clickedPos - mousePos;
		dragCollier->UpdateWorld();
	}
	else if (KEY_UP(VK_LBUTTON)) {
		for (auto robot : robots) {
			if (!robot->Active())
				continue;

			if (dragCollier->GlobalScale().x < FLT_EPSILON && dragCollier->GlobalScale().y < FLT_EPSILON)
				robot->SetSelected(robot->GetCollider()->IsPointCollision(mousePos));
			else
				robot->SetSelected(robot->GetCollider()->IsCollision(dragCollier));
		}
		dragCollier->SetActive(false);
	}
}

void Scene0119::MoveRobots()
{
	for (auto robot : robots) {
		if (!robot->Active())
			continue;

		if (robot->IsSelected())
			robot->Pos() += (mousePos - postPos);
	}
}
