#include "Framework.h"
#include "SpawnPolygon.h"

SpawnPolygon::SpawnPolygon()
{
	vertices.resize(MAX_POINT + 1);
	indices.resize(MAX_POINT * 3);

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

SpawnPolygon::~SpawnPolygon()
{
}

void SpawnPolygon::Update()
{
	if (!isActive)
		return;

	lifeTime -= DELTA;
	if (lifeTime < 0.0f)
		isActive = false;
}

void SpawnPolygon::Render()
{
	if (!isActive)
		return;

	__super::Render();
}

void SpawnPolygon::Spawn()
{
	isActive = true;
	lifeTime = Random(MIN_LIFETIME, MAX_LIFETIME);

	float radius = Random(MIN_RADIUS, MAX_RADIUS);
	int point = Random(MIN_POINT, MAX_POINT);

	Float3 pos = {
		Random(-1.0f + radius, 1.0f - radius), 
		Random(-1.0f + radius, 1.0f - radius), 
		0.0f 
	};
	Float4 color = {
		Random(0.0f, 1.0f),
		Random(0.0f, 1.0f),
		Random(0.0f, 1.0f),
		1.0f
	};

	vertices.resize(point+1);
	indices.resize(point*3);

	vertices[0].pos = pos;
	vertices[0].color = color;

	float angle = XM_2PI / point;

	for (UINT i = 0; i < point; i++) {
		Float3 tempPos = {};
		tempPos.x = pos.x + cosf(angle * i) * radius;
		tempPos.y = pos.y + sinf(angle * i) * radius;

		vertices[i + 1].pos = tempPos;
		vertices[i + 1].color = color;
	}

	for (UINT i = 0; i < point; i++) {
		indices[i * 3] = 0;
		indices[i * 3 + 1] = i+2 >= vertices.size() ? 1 : i+2;
		indices[i * 3 + 2] = i+1;
	}

	vertexBuffer->Update(vertices.data(), vertices.size());
	indexBuffer->Update(indices.data(), indices.size());
}
