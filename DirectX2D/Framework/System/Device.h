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
	IDXGISwapChain* GetSwapChain() { return swapChain; }
private:
	ID3D11Device* device;                       //CPU 접근(메모리를 비롯한 출력 외
	ID3D11DeviceContext* deviceContext;         //GPU 접근(출력)

	IDXGISwapChain* swapChain;                  //DX내부적으로 돌아가는 후면버퍼 관리(=더블 버퍼)
	ID3D11RenderTargetView* renderTargetView;	//VRAM 후면버퍼 메모리 관리
};

