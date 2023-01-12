#pragma once
class Device : public Singleton<Device>
{
private:
	friend class Singleton;
	Device();
	~Device();

public:
	void Clear();
	void Present();

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }

private:
	ID3D11Device* device;                       //CPU ����(�޸𸮸� ����� ��� ��
	ID3D11DeviceContext* deviceContext;         //GPU ����(���)

	IDXGISwapChain* swapChain;                  //DX���������� ���ư��� �ĸ���� ����(=���� ����)
	ID3D11RenderTargetView* renderTargetView;	//VRAM �ĸ���� �޸� ����
};
