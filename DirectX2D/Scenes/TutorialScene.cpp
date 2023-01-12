#include "Framework.h"
#include "TutorialScene.h"
#include "Homework/Polygon.h"

TutorialScene::TutorialScene()
{
	rect = new Rect({0, 0}, {100.0f, 100.0f});
	
	//왼손 좌표계로 원점이 센터로 맞춰진 설정을 끄겠다
	Matrix orthographic = XMMatrixOrthographicOffCenterLH(
		0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);

	worldBuffer = new MatrixBuffer();

	projectionBuffer = new MatrixBuffer();
	projectionBuffer->Set(orthographic);
	projectionBuffer->SetVS(2);

	worldMatrix._11 = 1;
	worldMatrix._22 = 1;
	worldMatrix._33 = 1;
	worldMatrix._44 = 1;

	pos = { CENTER_X, CENTER_Y };
	scale = { 1.0f, 1.0f };
}

TutorialScene::~TutorialScene()
{
	delete rect;
	delete worldBuffer;
	delete projectionBuffer;
}

void TutorialScene::Update()
{
	if (KEY_PRESS(VK_RIGHT))
		pos.x += 100.0f * DELTA;
	if (KEY_PRESS(VK_LEFT))
		pos.x -= 100.0f * DELTA;
	if (KEY_PRESS(VK_UP))
		pos.y += 100.0f * DELTA;
	if (KEY_PRESS(VK_DOWN))
		pos.y -= 100.0f * DELTA;

	if (KEY_PRESS('A'))
		scale.x -= DELTA;
	if (KEY_PRESS('D'))
		scale.x += DELTA;
	if (KEY_PRESS('W'))
		scale.y += DELTA;
	if (KEY_PRESS('S'))
		scale.y -= DELTA;
	if (KEY_PRESS('I'))
		angle += DELTA;
	if (KEY_PRESS('K'))
		angle -= DELTA;

	Matrix S = XMMatrixScaling(scale.x, scale.y, 1.0f);
	Matrix R = XMMatrixRotationZ(angle);
	Matrix T = XMMatrixTranslation(pos.x, pos.y, 0.0f);

	Matrix world = S * R * T;	//크기, 회전, 위치 - 통상적으로 이렇게 쓴다

	worldBuffer->Set(world);
	worldBuffer->SetVS(0);
}

void TutorialScene::Render()
{
	rect->Render();
}

void TutorialScene::PostRender()
{
}
