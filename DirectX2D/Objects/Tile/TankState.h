#pragma once
class TankState
{
private:
	const float RANGE = 200.0f;
	const float STOP_TIME = 1.0f;

public:
	TankState(Transform* tank);
	virtual ~TankState() = default;

	virtual void Update() {}
	virtual void Render() {}

	virtual void SetDestPos(Vector2 pos) {}

	virtual void Start() {}
protected:
	float speed = 100.0f;

	Transform* tank = nullptr;
	Vector2 destPos;
};

