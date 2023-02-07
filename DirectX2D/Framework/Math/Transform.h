#pragma once
class Transform
{
public:
	Transform();
	virtual ~Transform() = default;

	virtual void UpdateWorld();
	virtual void RenderUI();

	Vector2 Right() { return right.GetNormalized(); }
	Vector2 Left() { return right.GetNormalized() * -1.0f; }
	Vector2 Up() { return up.GetNormalized(); }
	Vector2 Down() { return up.GetNormalized() * -1.0f; }

	Vector2 GlobalPos() { 
		return globalPosition; 
	}
	Vector2 GlobalScale() { return globalScale; }

	bool Active();
	bool ActiveSelf() { return isActive; }

	void SetActive(bool isActive) {	this->isActive = isActive;	}
	void SetTag(string tag) { this->tag = tag; }

	Vector2& Pos() { return localPosition; }
	Float3& Rot() { return localRotation; }
	Vector2& Scale() { return localScale; }

	Transform* GetParent();
	Matrix GetWorld() { return world; }

	void SetParent(Transform* transform) { parent = transform; }
	void SetPivot(Vector2 pivot) { this->pivot = pivot; }

	void Load();
private:
	void Save();

protected:
	string tag;

	Vector2 localPosition;				//��ǥ
	Float3 localRotation = {0.0f, 0.0f, 0.0f};				//ȸ��
	Vector2 localScale = { 1.0f, 1.0f };	//����
protected:
	bool isActive = true;

	Matrix world;

private:
	Matrix S = {}, R = {}, T = {};

	Matrix P = {}, IP = {};
	Vector2 pivot; 	//���� ������

	Vector2 right, up;
	Vector2 globalPosition, globalScale;

	XMFLOAT4X4 matWorld = {};

	Transform* parent = nullptr;
};

