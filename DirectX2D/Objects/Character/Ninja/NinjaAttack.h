#pragma once
class NinjaAttack : public Action
{
public:
	NinjaAttack(string file, Transform* target);
	~NinjaAttack();

	virtual void Render() override;
	virtual void End() override;
protected:
	virtual void EnableCollider();
	virtual void DisableCollider();
	
protected:
	Transform* target;
	float power = 0.0f;

	RectCollider* attackCollider = nullptr;
};

