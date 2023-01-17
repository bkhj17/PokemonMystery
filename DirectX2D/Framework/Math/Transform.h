#pragma once
class Transform
{
public:
	Transform();
	~Transform() = default;

	void UpdateWorld();
	void RenderUI();

	Vector2 Right() { return right.GetNormalized(); }
	Vector2 Left() { return Right() * -1.0f; }
	Vector2 Up() { return up.GetNormalized(); }
	Vector2 Down() { return Up() * -1.0f; }

	Vector2 GlobalPos() { return globalPosition; }
	Vector2 GlobalScale() { return globalScale; }

	bool Active();
	bool ActiveSelf() { return isActive; }

	void SetActive(bool isActive) {	this->isActive = isActive;	}
	void SetTag(string tag) { this->tag = tag; }

	Vector2& Pos() { return localPosition; }
	Float3& Rotation() { return localRotation; }
	Vector2& Scale() { return localScale; }

	void SetParent(Transform* transform) { parent = transform; }
	void SetPivot(Vector2 pivot) { this->pivot = pivot; }
protected:
	string tag;

	Vector2 localPosition;				//좌표
	Float3 localRotation = {0.0f, 0.0f, 0.0f};				//회전
	Vector2 localScale = { 1.0f, 1.0f };	//배율
protected:
	bool isActive = true;

	Matrix world;

private:
	Matrix S = {}, R = {}, T = {};

	Matrix P = {}, IP = {};
	Vector2 pivot; 	//연산 기준점

	Vector2 right, up;
	Vector2 globalPosition, globalScale;

	XMFLOAT4X4 matWorld = {};

	Transform* parent = nullptr;
};

