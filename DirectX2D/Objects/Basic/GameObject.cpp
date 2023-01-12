#include "Framework.h"

GameObject::GameObject(wstring shaderFile)
{
	vertexShader = Shader::AddVS(L"Vertex" + shaderFile);
	pixelShader = Shader::AddPS(L"Pixel" + shaderFile);
}

GameObject::~GameObject()
{
	if(vertexBuffer != nullptr)
		delete vertexBuffer;
	if(indexBuffer != nullptr)
		delete indexBuffer;
}

void GameObject::Render()
{
	if(vertexBuffer != nullptr)
		vertexBuffer->Set();
	if (indexBuffer != nullptr)
		indexBuffer->Set();

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}
