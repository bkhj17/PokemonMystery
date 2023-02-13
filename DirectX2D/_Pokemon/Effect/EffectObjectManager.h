#pragma once

class EffectObject;
class EffectObjectManager : public Singleton<EffectObjectManager>
{
private:
	const UINT POOL_SIZE = 100.0f;

	friend class Singleton;
	EffectObjectManager();
	~EffectObjectManager();

public:
	void Update();
	void Render();

	EffectObject* Pop();

private:
	vector<EffectObject*> effects;
};

