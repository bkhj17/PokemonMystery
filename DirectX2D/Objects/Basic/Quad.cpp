#include "Framework.h"

Quad::Quad(Vector2 size, Vector2 pos)
	: size(size)
{
	CreateMesh(size, pos);
}

Quad::Quad(wstring file, Vector2 pos)
{
	SetTexture(file);
	size = texture->GetSize();
	CreateMesh(size, pos);
}

Quad::~Quad()
{
}

void Quad::Render()
{
	texture->PSSet();
	__super::Render();
}

void Quad::SetTexture(wstring file)
{
	texture = Texture::Add(file);
}

void Quad::CreateMesh(Vector2 size, Vector2 pos)
{
	float left = pos.x - size.x * 0.5f;
	float right = pos.x + size.x * 0.5f;
	float top = pos.y + size.y * 0.5f;
	float bottom = pos.y - size.y * 0.5f;

	vertices.emplace_back(left, top, 0.0f, 0.0f);
	vertices.emplace_back(right, top, 1.0f, 0.0f);
	vertices.emplace_back(left, bottom, 0.0f, 1.0f);
	vertices.emplace_back(right, bottom, 1.0f, 1.0f);

	indices = { 0, 1, 2, 2, 1, 3 };

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), (UINT)vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), (UINT)indices.size());
}
