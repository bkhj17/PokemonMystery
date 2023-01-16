#pragma once
class Area0116 : public Quad
{
public:
	Area0116(Vector2 size, Vector2 pos = {});
	~Area0116();

	Vector2 LeftTop();
	Vector2 RightBottom();
};

