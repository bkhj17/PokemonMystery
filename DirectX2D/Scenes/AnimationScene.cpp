#include "Framework.h"
#include "AnimationScene.h"

AnimationScene::AnimationScene()
{
	ninja = new Ninja;
	BulletManager::Get();
}

AnimationScene::~AnimationScene()
{
	delete ninja;
	BulletManager::Delete();
}

void AnimationScene::Update()
{
	ninja->Update();

	BulletManager::Get()->Update();
}

void AnimationScene::Render()
{
	ninja->Render();
	
	BulletManager::Get()->Render();
}

void AnimationScene::PostRender()
{
}
