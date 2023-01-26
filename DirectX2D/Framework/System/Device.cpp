#include "Framework.h"
#include "Device.h"

Device::Device()
{
	UINT width = WIN_WIDTH;
	UINT height = WIN_HEIGHT;

#pragma region CreateSwapChain
	//버퍼 정보
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //RGBA 8비트씩. UNORM = unsigned normal(0~1)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	//RefreshRate : 디스플레이 모드 갱신률(주사율 : Numerator / Denominator)

	//표본화 계단현상을 막기 위해 출력할 리소스의 표본을 만들어두는 방식
	//최대표본화 : 엄청 키워놓고 축소 시켜서 사용(메모리 많이 잡아먹어서 dx에선 안 씀)
	//다중표본화 : 여러개 만들어놓고 씀(복잡해서 게임 프로그래밍에서 쓰기엔 부담스러움)
	swapChainDesc.SampleDesc.Count = 1;     //1개
	swapChainDesc.SampleDesc.Quality = 0;   //안 쓴다
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;          //백버퍼 개수 
	swapChainDesc.OutputWindow = hWnd;      //윈도우 정보
	swapChainDesc.Windowed = true;          //창모드 / 전체모드

	D3D11CreateDeviceAndSwapChain(
		nullptr,                            //어댑터 : 실시간 화면 크기 조절에 필요. 
		D3D_DRIVER_TYPE_HARDWARE,           //접근할 드라이버. 에뮬레이터 사용 시 소프트웨어 모드
		0,                                  //사용할 소프트웨어. 0 == 사용 안 함
		D3D10_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr,                            //사양 체크
		0,
		D3D11_SDK_VERSION,                  //개발 도구 버전
		&swapChainDesc,                     //위에서 넣은 정보들
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	);
#pragma endregion

#pragma region CreateRenderTargetView
	ID3D11Texture2D* backBuffer = nullptr;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
	backBuffer->Release();
#pragma endregion
	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
}

Device::~Device()
{
	renderTargetView->Release();
	swapChain->Release();

	deviceContext->Release();
	device->Release();
}

void Device::Clear()
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

	float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	deviceContext->ClearRenderTargetView(renderTargetView, clearColor); //렌더타겟뷰 초기화
}

void Device::Present()
{
	//백버퍼 스왑
	swapChain->Present(0, 0);   //출력 끝나고 다음 출력 준비
}
