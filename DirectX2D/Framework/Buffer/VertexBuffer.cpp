#include "Framework.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count)
	: stride(stride)
{
#pragma region CreateBuffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;             //CPU, GPU에 대한 접근 권한
	bufferDesc.ByteWidth = stride * count;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    //버퍼 용도

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data; //시작 주소

	DEVICE->CreateBuffer(&bufferDesc, &initData, &buffer);
#pragma endregion
}

VertexBuffer::~VertexBuffer()
{
	buffer->Release();
}

void VertexBuffer::Set(D3D11_PRIMITIVE_TOPOLOGY type)
{
	//IA 세팅
	DC->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	DC->IASetPrimitiveTopology(type);
}

void VertexBuffer::Set(UINT slot, D3D11_PRIMITIVE_TOPOLOGY type)
{
	DC->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
	DC->IASetPrimitiveTopology(type);
}

void VertexBuffer::Update(void* data, UINT count)
{
	DC->UpdateSubresource(buffer, 0, nullptr, data, stride, count);
}
