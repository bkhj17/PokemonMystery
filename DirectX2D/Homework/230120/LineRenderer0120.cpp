#include "Framework.h"
#include "LineRenderer0120.h"

LineRenderer0120::LineRenderer0120()
{
	vertexShader = Shader::AddVS(L"VertexPos.hlsl");
	pixelShader = Shader::AddPS(L"PixelPos.hlsl");

	vertices.resize(8);
	vertexBuffer = new VertexBuffer(vertices.data(), (UINT)sizeof(VertexPos), (UINT)vertices.size());
	colorBuffer = new ColorBuffer;
	colorBuffer->Get() = { 1.0f,0.0f,0.0f,1.0f };
}

LineRenderer0120::~LineRenderer0120()
{
	delete vertexBuffer;
	delete colorBuffer;
}

void LineRenderer0120::Set(deque<Vector2>& dq)
{
	vertices.clear();
	while (!dq.empty()) {
		vertices.emplace_back(dq.front().x, dq.front().y);
		dq.pop_front();
	}
	vertexBuffer->Update(vertices.data(), (UINT)sizeof(vertices));
}

void LineRenderer0120::Render()
{
	colorBuffer->SetPS(0);

	vertexBuffer->Set(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	vertexShader->Set();
	pixelShader->Set();

	DC->Draw((UINT)vertices.size(), 0);
}
