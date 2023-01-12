#pragma once
class Transform0112
{
public:
	Transform0112();
	~Transform0112();


	Vector2 pos = {};				//좌표
	Vector2 scale = { 1.0f, 1.0f };	//배율
	float angle = 0.0f;				//회전

	Matrix GetWorldMatrix();
	void Set();
private:
	MatrixBuffer* worldBuffer;
};

