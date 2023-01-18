#pragma once

class RobotAttack0118 : public Action
{
public:
	RobotAttack0118(string file, Transform* target);
	~RobotAttack0118();

	virtual void Update() override;

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

