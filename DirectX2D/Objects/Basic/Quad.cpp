#include "Framework.h"

Quad::Quad(Vector2 size, Vector2 pos)
	: size(size)
{
	CreateMesh(size, {}, {1.0f, 1.0f});

	colorBuffer = new ColorBuffer();
}

Quad::Quad(wstring file, Vector2 startUV, Vector2 endUV)
{
	SetTexture(file);
	size = (endUV - startUV) * texture->GetSize();
	CreateMesh(size, startUV, endUV);
	
	colorBuffer = new ColorBuffer();
}

Quad::~Quad()
{
	delete vertexBuffer;
	delete indexBuffer;
	delete colorBuffer;
}

void Quad::Render()
{
	if (!isActive)
		return;

	if(texture != nullptr)
		texture->PSSet();

	vertexBuffer->Set();
	indexBuffer->Set();

	colorBuffer->SetPS(0);

	SetRender();

	DC->DrawIndexed((UINT)indices.size(), 0, 0);
}

void Quad::SetTexture(wstring file)
{
	texture = Texture::Add(file);
}

void Quad::CreateMesh(Vector2 size, Vector2 startUV, Vector2 endUV)
{
	float left = -size.x * 0.5f;
	float right = size.x * 0.5f;
	float top = size.y * 0.5f;
	float bottom = -size.y * 0.5f;

	vertices.emplace_back(left, top, startUV.x, startUV.y);
	vertices.emplace_back(right, top, endUV.x, startUV.y);
	vertices.emplace_back(left, bottom, startUV.x, endUV.y);
	vertices.emplace_back(right, bottom, endUV.x, endUV.y);

	indices = { 0, 1, 2, 2, 1, 3 };

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), (UINT)vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), (UINT)indices.size());

}