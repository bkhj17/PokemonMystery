#include "Framework.h"
#include "Rect.h"

Rect::Rect(Float2 pos, Float2 size)
{
	vertexShader = Shader::AddVS(L"VertexShader.hlsl");
	pixelShader = Shader::AddPS(L"PixelShader.hlsl");

	float left = pos.x - size.x * 0.5f;
	float right = pos.x + size.x * 0.5f;
	float top = pos.y + size.y * 0.5f;
	float bottom = pos.y - size.y * 0.5f;

	vertices.emplace_back(left, top, 1.0f, 0.0f, 0.0f);
	vertices.emplace_back(right, top, 1.0f, 1.0f, 0.0f);
	vertices.emplace_back(left, bottom, 1.0f, 0.0f, 1.0f);
	vertices.emplace_back(right, bottom, 1.0f, 1.0f, 0.0f);

	indices = { 0, 1, 2, 2, 1, 3 };

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

Rect::~Rect()
{
	delete vertexBuffer;
	delete indexBuffer;
}

void Rect::Update()
{
}

void Rect::Render()
{
	if (!isActive)
		return;

	vertexBuffer->Set();
	indexBuffer->Set();

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}
