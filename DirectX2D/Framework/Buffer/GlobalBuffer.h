#pragma once
class MatrixBuffer : public ConstBuffer 
{
public:
	MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix)) {
		matrix = XMMatrixIdentity(); 
	}

	void Set(Matrix value) {
		matrix = XMMatrixTranspose(value);
	}

private:
	Matrix matrix;
};

class ColorBuffer : public ConstBuffer
{
public:
	ColorBuffer() : ConstBuffer(&color, sizeof(Float4)) {}

	Float4& Get() {	return color; }

private:
	Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
};