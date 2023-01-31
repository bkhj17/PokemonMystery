#pragma once

class Window0131 : public Quad
{
public:
	Window0131(Vector2 size);
	virtual ~Window0131();

	virtual void Update();
	virtual void UpdateWorld() override;
	virtual void Render() override;

	void Close();

	RectCollider* GetCollider() { return collider; }
private:
	RectCollider* collider;
	Button* closeButton;
};

