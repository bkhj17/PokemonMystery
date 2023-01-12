#include "Framework.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(void* data, UINT count)
{
#pragma region CreateBuffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;             //CPU, GPU에 대한 접근 권한
	bufferDesc.ByteWidth = sizeof(UINT) * count;		//버퍼 메모리 크기
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;		//버퍼 용도

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data; //시작 주소

	DEVICE->CreateBuffer(&bufferDesc, &initData, &buffer);
#pragma endregion
}

IndexBuffer::~IndexBuffer()
{
	buffer->Release();
}

void IndexBuffer::Set()
{
	DC->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer::Update(void* data, UINT count)
{	
	DC->UpdateSubresource(buffer, 0, nullptr, data, sizeof(UINT), count);
}
