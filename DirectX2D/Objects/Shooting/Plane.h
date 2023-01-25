#pragma once
class Plane : public Quad
{
public:
	Plane();
	~Plane();

	void Update();
	void Render();
private:
	void Move();
	void Rotate();
	void Fire();
public:
	char upKey = 'W';
	char downKey = 'S';
	char leftKey = 'A';
	char rightKey = 'D';

private:
	float speed = 1500.0f;
	float turnSpeed = 3.0f;

	Quad* weapon;
};

