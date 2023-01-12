#include "Framework.h"
#include "ConstBuffer.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
	: data(data), dataSize(dataSize)
{
#pragma region CreateBuffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;             //CPU, GPU에 대한 접근 권한
	bufferDesc.ByteWidth = dataSize;					//버퍼 메모리 크기
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//버퍼 용도

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data; //시작 주소

	DEVICE->CreateBuffer(&bufferDesc, nullptr, &buffer);
#pragma endregion
}

ConstBuffer::~ConstBuffer()
{
	buffer->Release();
}

void ConstBuffer::SetVS(UINT slot)
{
	DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
	DC->VSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetPS(UINT slot)
{
	DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
	DC->PSSetConstantBuffers(slot, 1, &buffer);
}
