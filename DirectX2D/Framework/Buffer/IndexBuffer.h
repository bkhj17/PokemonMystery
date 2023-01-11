#pragma once
class IndexBuffer
{
public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	void Set();
private:
	ID3D11Buffer* buffer;

	UINT stride = 0;
	UINT offset = 0;
};

