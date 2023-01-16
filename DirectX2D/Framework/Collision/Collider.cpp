#include "Framework.h"
#include "Collider.h"

Collider::Collider()
{
	vertexShader = Shader::AddVS(L"VertexPos.hlsl");
	pixelShader = Shader::AddPS(L"PixelPos.hlsl");

	worldBuffer = new MatrixBuffer;
	colorBuffer = new ColorBuffer;
	colorBuffer->Get() = { 0.0f,1.0f,0.0f,1.0f };
}

Collider::~Collider()
{
	delete vertexBuffer;
	delete worldBuffer;
	delete colorBuffer;
}

void Collider::Render()
{
	if (!isActive)
		return;

	worldBuffer->Set(world);
	worldBuffer->SetVS(0);
	colorBuffer->SetPS(0);

	vertexBuffer->Set(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	vertexShader->Set();
	pixelShader->Set();

	DC->Draw((UINT)vertices.size(), 0);
}

bool Collider::IsCollision(Collider* collider)
{
	if (!Active() || !collider->Active())
		return false;

	switch (collider->type)
	{
	case Collider::Type::RECT:
		return IsRectCollision((RectCollider*)collider);
	case Collider::Type::CIRCLE:
		return IsCircleCollision((CircleCollider*)collider);
	}

	return false;
}
