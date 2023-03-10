#pragma once
class GameObject : public Transform
{
public:
	GameObject(wstring shaderFile = L"UV.hlsl");
	virtual ~GameObject();

	virtual void SetRender();

	void SetVertexShader(wstring shaderFile);
	void SetPixelShader(wstring shaderFile);

	virtual Vector2 GetSize() { return size; }
	virtual Vector2 GetGlobalSize() { return size * GlobalScale(); }
protected:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	MatrixBuffer* worldBuffer;

	Vector2 size;
};

