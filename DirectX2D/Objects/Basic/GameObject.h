#pragma once
class GameObject
{
public:
	GameObject(wstring shaderFile = L"Shader.hlsl");
	virtual ~GameObject();

	virtual void Update() {}
	virtual void Render();

protected:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	vector<VertexColor> vertices;
	vector<UINT> indices;
};

