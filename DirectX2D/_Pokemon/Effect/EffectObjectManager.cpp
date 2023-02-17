#include "Framework.h"

EffectObjectManager::EffectObjectManager()
{
	effects.resize(POOL_SIZE);
	for (auto& e : effects)
		e = new EffectObject();
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

bool EffectObjectManager::IsActing()
{
	for (auto e : effects)
		if (e->Active())
			return true;
	return false;
}
