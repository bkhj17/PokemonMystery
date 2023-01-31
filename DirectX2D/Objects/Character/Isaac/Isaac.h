#pragma once
class Isaac : public RectCollider
{
private:
	enum State {
		UP, DOWN, LEFT, RIGHT
	};
public:
	Isaac();
	~Isaac();

	void Update();
	void Render();

private:
	void Move();

	void CreateClips();

private:
	AnimObject* head;
	AnimObject* body;

	float speed = 100.0f;
};

