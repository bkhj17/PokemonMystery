#include "Framework.h"
#include "EffectManager.h"

EffectManager::~EffectManager()
{
	//할당된 이펙트 삭제
	for (auto& effects : totalEffects) {
		for (auto effect : effects.second)
			delete effect;
	}
}

void EffectManager::Update()
{
	for (auto& effects : totalEffects) {
		for (auto effect : effects.second)
			effect->Update();
	}
}

void EffectManager::Render()
{
	for (auto& effects : totalEffects) {
		for (auto effect : effects.second)
			effect->Render();
	}
}

void EffectManager::Add(string key, UINT poolSize, wstring textureFile, float maxFrameX, float maxFrameY, float speed, bool isAdditive)
{
	if (totalEffects.find(key) != totalEffects.end())
		return;

	Effects effects(poolSize);
	for (auto& effect : effects)
		effect = new Effect(textureFile, { maxFrameX, maxFrameY }, speed, isAdditive);
	totalEffects[key] = effects;
}

void EffectManager::Play(string key, Vector2 pos)
{
	if (totalEffects.find(key) == totalEffects.end())
		return;

	for (auto effect : totalEffects[key]) {
		if (!effect->Active()) {
			effect->Play(pos);
			return;
		}
	}
}

void EffectManager::SetColor(string key, float r, float g, float b, float a)
{
	if (totalEffects.find(key) == totalEffects.end())
		return;

	for (auto effect : totalEffects[key]) {
		effect->GetColor() = { r, g, b, a };
	}
}
