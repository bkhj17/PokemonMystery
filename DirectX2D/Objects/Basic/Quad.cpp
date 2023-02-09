#include "Framework.h"

Quad::Quad(Vector2 size)
	: startUV(Vector2(0, 0)), endUV(Vector2(1, 1))
{
	this->size = size;
	CreateMesh();

	colorBuffer = new ColorBuffer();
}

Quad::Quad(wstring file, Vector2 startUV, Vector2 endUV)
	: startUV(startUV), endUV(endUV)
{
	SetTexture(file);
	size = (endUV - startUV) * texture->GetSize();
	CreateMesh();
	
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
	if (!CAM->ContainFrustum(GlobalPos(), GetGlobalSize()))
		return;
	SetRender();
	DC->DrawIndexed((UINT)indices.size(), 0, 0);
}

void Quad::PostRender()
{
	if (!isActive)
		return;

	SetRender();
	DC->DrawIndexed((UINT)indices.size(), 0, 0);
}

void Quad::SetRender()
{
	if (texture != nullptr)
		texture->PSSet();

	vertexBuffer->Set();
	indexBuffer->Set();

	colorBuffer->SetPS(0);

	GameObject::SetRender();
}

void Quad::SetTexture(wstring file)
{
	texture = Texture::Add(file);
}

void Quad::SetSize(Vector2 size)
{
	this->size = size; 
	UpdateVertices();
}

void Quad::UpdateVertices()
{
	float left = -size.x * 0.5f;
	float right = +size.x * 0.5f;
	float top = +size.y * 0.5f;
	float bottom = -size.y * 0.5f;

	vertices[0] = Vertex(left, top, startUV.x, startUV.y);
	vertices[1] = Vertex(right, top, endUV.x, startUV.y);
	vertices[2] = Vertex(left, bottom, startUV.x, endUV.y);
	vertices[3] = Vertex(right, bottom, endUV.x, endUV.y);

	vertexBuffer->Update(vertices.data(), vertices.size());
}

void Quad::CreateMesh()
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

void Quad::ModifyUV(Vector2 startUV, Vector2 endUV)
{
	this->startUV = startUV;
	this->endUV = endUV;

	vertices[0].uv = { startUV.x, startUV.y };
	vertices[1].uv = { endUV.x, startUV.y };
	vertices[2].uv = { startUV.x, endUV.y };
	vertices[3].uv = { endUV.x, endUV.y };

	vertexBuffer->Update(vertices.data(), (UINT)vertices.size());
}
