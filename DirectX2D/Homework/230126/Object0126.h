#pragma once

class Object0126 : public Quad
{
public:
	Object0126(wstring file);
	virtual ~Object0126();

	virtual void Update();
	virtual void Render() override;

	RectCollider* GetCollider() { return rectCollider; }
protected:
	RectCollider* rectCollider;
};

