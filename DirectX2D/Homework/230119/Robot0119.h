#pragma once
#include "../230118/Robot/Robot0118.h"

class Robot0119 : public Robot0118
{
public:
	Robot0119();
	~Robot0119();

	virtual void Update() override;
	virtual void Render() override;

	void SetSelected(bool selected) { this->selected = selected; }
	bool IsSelected() { return selected; }

	Vector2 GetSize();

	RectCollider* GetCollider() { return collider; }
private:
	bool selected = false;

	FloatValueBuffer* floatValueBuffer;

	RectCollider* collider;
};

