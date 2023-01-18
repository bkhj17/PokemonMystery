#pragma once

class NinjaThrow : public Action
{
public:
	NinjaThrow(Transform* target);
	~NinjaThrow() = default;

	virtual void End();
private:
	void Fire();

private:
	Transform* target;
};

