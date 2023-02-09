#include "Framework.h"

GameObject::GameObject(wstring shaderFile)
{
	vertexShader = Shader::AddVS(L"Vertex" + shaderFile);
	pixelShader = Shader::AddPS(L"Pixel" + shaderFile);

	worldBuffer = new MatrixBuffer();
}

GameObject::~GameObject()
{
	delete worldBuffer;
}

void GameObject::SetRender()
{
	worldBuffer->Set(world);
	worldBuffer->SetVS(0);

	vertexShader->Set();

	auto shader = Shader::AddPS(L"PixelPokemonProgressBar.hlsl");
	pixelShader->Set();
}

void GameObject::SetVertexShader(wstring shaderFile)
{
	vertexShader = Shader::AddVS(L"Vertex" + shaderFile);
}

void GameObject::SetPixelShader(wstring shaderFile)
{
	pixelShader = Shader::AddPS(L"Pixel" + shaderFile);
}
