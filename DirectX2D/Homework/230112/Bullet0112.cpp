#include "Framework.h"
#include "Bullet0112.h"
#include "Transform0112.h"

Bullet0112::Bullet0112()
{
	int point = 10;

	Float4 color = { Random(0.0f, 1.0f), Random(0.0f, 1.0f), Random(0.0f, 1.0f), 1.0f};

	vertices.resize(point+1);
	indices.resize(point*3);

	vertices[0].pos = { 0.0f, 0.0f, 0.0f };
	vertices[0].color = color;

	float angle = XM_2PI / point;
	for (UINT i = 0; i < point; i++) {
		Float3 tempPos = {};
		tempPos.x = cosf(angle * i) * RADIUS;
		tempPos.y = sinf(angle * i) * RADIUS;

		vertices[i + 1].pos = tempPos;
		vertices[i + 1].color = color;
	}
	for (UINT i = 0; i < point; i++) {
		indices[i * 3] = 0;
		indices[i * 3 + 1] = i + 2 >= vertices.size() ? 1 : i + 2;
		indices[i * 3 + 2] = i + 1;
	}

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	transform = new Transform0112();
}

Bullet0112::~Bullet0112()
{
	delete transform;
}

void Bullet0112::Update()
{
	if (!isActive)
		return;

	transform->pos.x += cosf(transform->angle) * DELTA * SPEED;
	transform->pos.y += sinf(transform->angle) * DELTA * SPEED;


	//화면 나가면 끄기
	if (transform->pos.x + RADIUS < 0.0f || transform->pos.x - RADIUS > WIN_WIDTH
		|| transform->pos.y + RADIUS < 0.0f || transform->pos.y - RADIUS > WIN_HEIGHT)
		isActive = false;

}

void Bullet0112::Render()
{
	if (!isActive)
		return;

	transform->Set();
	__super::Render();
}

void Bullet0112::Spawn(Vector2 pos, float angle)
{
	isActive = true;
	transform->pos = pos;
	transform->angle = angle;
}
