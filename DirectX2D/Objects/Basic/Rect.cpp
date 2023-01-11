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
	if (!isActive)
		return;

	lifeTime += DELTA;
	if (lifeTime >= timeToDeath)
		isActive = false;
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

void Rect::Spawn(Float2 pos, Float2 size)
{
	isActive = true;

	float left = pos.x - size.x * 0.5f;
	float right = pos.x + size.x * 0.5f;
	float top = pos.y + size.y * 0.5f;
	float bottom = pos.y - size.y * 0.5f;

	Float3 vpos[] = {
		{left, top, 0.0f},
		{ right, top, 0.0f },
		{ left, bottom, 0.0f },
		{ right, bottom, 0.0f }
	};

	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].pos = vpos[i];
		vertices[i].color = { Random(0.0f, 1.0f), Random(0.0f, 1.0f), Random(0.0f, 1.0f), 1.0f };
	}
	vertexBuffer->Update(vertices.data(), vertices.size());

	timeToDeath = Random(0.5f, 2.0f);
	lifeTime = 0.0f;
}
