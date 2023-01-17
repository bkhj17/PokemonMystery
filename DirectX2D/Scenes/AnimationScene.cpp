#include "Framework.h"
#include "AnimationScene.h"

AnimationScene::AnimationScene()
{
	ninja = new Ninja;
}

AnimationScene::~AnimationScene()
{
	delete ninja;
}

void AnimationScene::Update()
{
	ninja->Update();
}

void AnimationScene::Render()
{
	ninja->Render();
}

void AnimationScene::PostRender()
{
}
