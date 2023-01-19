#pragma once
class RectCollider : public Collider
{
public:
	struct ObbDesc {
		Vector2 position;
		Vector2 axis[2];
		Vector2 halfSize;
	};

public:
	RectCollider(Vector2 size);
	~RectCollider() = default;
	// Collider을(를) 통해 상속됨
	virtual bool IsPointCollision(Vector2 point) override;
	virtual bool IsRectCollision(RectCollider* rect, Vector2* overlap) override;
	virtual bool IsCircleCollision(CircleCollider* circle) override;
	bool IsCircleCollision(CircleCollider* circle, Vector2* overlap);

	Vector2 Size() { return { size.x * GlobalScale().x, size.y * GlobalScale().y }; }
	Vector2 Half() { return Size() * 0.5f; }

	Vector2 LeftTop();
	Vector2 LeftBottom();
	Vector2 RightTop();
	Vector2 RightBottom();

	float L();
	float R();
	float T();
	float B();

	ObbDesc GetObb();

private:
	bool IsAABB(RectCollider* rect, Vector2* overlap);//AABB
	bool IsOBB(RectCollider* rect);//OBB

	bool IsSeperate(Vector2 sperateAxis, ObbDesc box1, ObbDesc box2);

private:
	Vector2 size;

	ObbDesc obbDesc = {};
};

