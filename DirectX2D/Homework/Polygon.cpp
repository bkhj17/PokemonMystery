#include "Framework.h"
#include "Polygon.h"

SpawnPolygon0111::SpawnPolygon0111()
{
	vertexShader = Shader::AddVS(L"VertexShader.hlsl");
	pixelShader = Shader::AddPS(L"PixelShader.hlsl");


	vertices.resize(POOLSIZE);
	indices.resize(POOLSIZE*3);

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

SpawnPolygon0111::~SpawnPolygon0111()
{
	delete vertexBuffer;
	delete indexBuffer;
}


void SpawnPolygon0111::Update()
{
	if (!isActive)
		return;

	lifeTime += DELTA;
	if (lifeTime >= timeToDeath)
		isActive = false;
}

void SpawnPolygon0111::Render()
{
	if (!isActive)
		return;

	vertexBuffer->Set();
	indexBuffer->Set();

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(use*3, 0, 0);
}

void SpawnPolygon0111::Spawn()
{
	isActive = true;
	Float2 pos = { Random(-0.5f, 0.5f), Random(-0.5f, 0.5f) };
	float size = Random(0.1f, 0.5f);
	use = Random(3, POOLSIZE-1);

	float startAngle = XM_PI * 0.5f;
	float addAngle = -XM_2PI / use;

	vertices[0].pos = { pos.x, pos.y, 0.0f };
	for (int i = 1; i <= use; i++) {
		float vAngle = startAngle + addAngle * (i-1);
		vertices[i].pos = { pos.x + size * cosf(vAngle), pos.y + size * sinf(vAngle), 0.0f };
		vertices[i].color = { Random(0.0f, 1.0f), Random(0.0f, 1.0f), Random(0.0f, 1.0f), 1.0f };;
	}

	int cnt = 0;
	for (int i = 1; i < use; i++) {
		indices[cnt++] = 0;
		indices[cnt++] = i;
		indices[cnt++] = i+1;
	}
	indices[cnt++] = 0;
	indices[cnt++] = use;
	indices[cnt++] = 1;

	vertexBuffer->Update(vertices.data(), use+1);
	indexBuffer->Update(indices.data(), use*3);

	timeToDeath = Random(0.5f, 2.0f);
	lifeTime = 0.0f;
}
