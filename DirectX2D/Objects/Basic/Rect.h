#pragma once
class Rect : public GameObject
{
public:
	Rect(Float2 pos, Float2 size);
	~Rect() = default;

	void Render();

private:

};

