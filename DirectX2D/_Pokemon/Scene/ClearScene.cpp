#include "Framework.h"
#include "ClearScene.h"

ClearScene::ClearScene()
{
	clearUI = new ClearUI();
	clearUI->SetActive(true);
}

ClearScene::~ClearScene()
{
	delete clearUI;
}

void ClearScene::Update()
{
	if (clearUI->Pos().y < CENTER_Y)
		clearUI->Pos().y += DELTA * 1000.0f;

	clearUI->Update();
}

void ClearScene::Render()
{
	clearUI->PostRender();
}

void ClearScene::PostRender()
{
	clearUI->PostRender();
}

void ClearScene::Start()
{
	clearUI->Pos().y = -800.0f;
}
