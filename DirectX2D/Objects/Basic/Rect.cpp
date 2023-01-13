#include "Framework.h"

Rect::Rect(Float2 pos, Float2 size)
{
	float left = pos.x - size.x * 0.5f;
	float right = pos.x + size.x * 0.5f;
	float top = pos.y + size.y * 0.5f;
	float bottom = pos.y - size.y * 0.5f;

	vertices.emplace_back(left, top, 1.0f, 0.0f, 0.0f);
	vertices.emplace_back(right, top, 0.0f, 1.0f, 0.0f);
	vertices.emplace_back(left, bottom, 0.0f, 0.0f, 1.0f);
	vertices.emplace_back(right, bottom, 1.0f, 1.0f, 0.0f);
	
	indices = { 0, 1, 2, 2, 1, 3 };

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

void Rect::Render()
{
	__super::Render();
}
