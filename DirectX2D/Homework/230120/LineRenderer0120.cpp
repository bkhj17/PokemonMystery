#include "Framework.h"
#include "LineRenderer0120.h"

LineRenderer0120::LineRenderer0120()
{
	vertexShader = Shader::AddVS(L"VertexPos.hlsl");
	pixelShader = Shader::AddPS(L"PixelPos.hlsl");

	vertices.resize(200);
	vertexBuffer = new VertexBuffer(vertices.data(), (UINT)sizeof(VertexPos), (UINT)vertices.size());


	worldBuffer = new MatrixBuffer;
	worldBuffer->Set(XMMatrixIdentity());

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

	showTime = showRate;
}

void LineRenderer0120::Update()
{
	if (showTime <= 0.0f)
		return;

	showTime -= DELTA;
}

void LineRenderer0120::Render()
{
	if (showTime <= 0.0f)
		return;

	worldBuffer->SetVS(0);
	colorBuffer->SetPS(0);

	vertexBuffer->Set(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	vertexShader->Set();
	pixelShader->Set();

	DC->Draw((UINT)vertices.size(), 0);
}
