#pragma once
class GameObject : public Transform
{
public:
	GameObject(wstring shaderFile = L"UV.hlsl");
	virtual ~GameObject();

	void SetRender();

protected:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	MatrixBuffer* worldBuffer;

};

