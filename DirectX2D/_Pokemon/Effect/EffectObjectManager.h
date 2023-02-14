#pragma once

class EffectObject;
class EffectObjectManager : public Singleton<EffectObjectManager>
{
private:
	const UINT POOL_SIZE = 100;

	friend class Singleton;
	EffectObjectManager();
	~EffectObjectManager();

public:
	void Update();
	void Render();

	EffectObject* Pop();

	bool IsActing();
private:
	vector<EffectObject*> effects;
};

