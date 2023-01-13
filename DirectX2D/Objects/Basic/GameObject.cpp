#include "Framework.h"

GameObject::GameObject(wstring shaderFile)
{
	vertexShader = Shader::AddVS(L"Vertex" + shaderFile);
	pixelShader = Shader::AddPS(L"Pixel" + shaderFile);

	worldBuffer = new MatrixBuffer();
}

GameObject::~GameObject()
{
	if(vertexBuffer != nullptr)
		delete vertexBuffer;
	if(indexBuffer != nullptr)
		delete indexBuffer;

	delete worldBuffer;
}

void GameObject::Update()
{
	if (!isActive)
		return;

	UpdateWorld();
}

void GameObject::Render()
{
	if (!isActive)
		return;

	worldBuffer->Set(world);
	worldBuffer->SetVS(0);

	if(vertexBuffer != nullptr)
		vertexBuffer->Set();
	if (indexBuffer != nullptr)
		indexBuffer->Set();

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}
