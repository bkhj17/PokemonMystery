#include "Framework.h"
#include "AnimationScene.h"

AnimationScene::AnimationScene()
{
	ninja = new Ninja;
	
	EffectManager::Get()->Add("exp1", 10, L"Textures/Effect/Explosion/Explosion1.png", 4.0f, 2.0f, 1.0f, true);
	EffectManager::Get()->SetColor("exp1", 1.0f, 1.0f, 1.0f, 1.0f);
	
	EffectManager::Get()->Add("exp2", 10, L"Textures/Effect/Explosion/Explosion2.png", 4.0f, 2.0f, 1.0f, true);
	EffectManager::Get()->SetColor("exp2", 1.0f, 1.0f, 1.0f, 1.0f);


	BulletManager::Get();

	
}

AnimationScene::~AnimationScene()
{
	delete ninja;
	delete effect;
	BulletManager::Delete();
	EffectManager::Delete();
}

void AnimationScene::Update()
{
	ninja->Update();
	
	if (KEY_DOWN('1')) {
		EffectManager::Get()->Play("exp1", mousePos);
	}
	if (KEY_DOWN('2')) {
		EffectManager::Get()->Play("exp2", mousePos);
	}


	BulletManager::Get()->Update();
	EffectManager::Get()->Update();
}

void AnimationScene::Render()
{
	ninja->Render();
	BulletManager::Get()->Render();
	EffectManager::Get()->Render();
}

void AnimationScene::PostRender()
{
	ninja->PostRender();
}
