#include "Framework.h"
#include "EffectObjectManager.h"
#include "EffectObject.h"

EffectObjectManager::EffectObjectManager()
{
	effects.resize(POOL_SIZE);
	for (auto& effect : effects) {
		effect = new EffectObject();
	}
}

EffectObjectManager::~EffectObjectManager()
{
	for (auto e : effects)
		delete e;
}

void EffectObjectManager::Update()
{
	for (auto e : effects) {
		if (e->Active())
			e->Update();
	}
}

void EffectObjectManager::Render()
{
	for (auto e : effects) {
		if (e->Active())
			e->Render();
	}
}

EffectObject* EffectObjectManager::Pop()
{
	for (auto e : effects) {
		if (e->Active())
			continue;

		return e;
	}

	return nullptr;
}
