// DirectX2D.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "DirectX2D.h"

#define MAX_LOADSTRING 100

struct Vertex //정점 : 3차원 공간에서의 한 점
{
    DirectX::XMFLOAT3 pos;

    Vertex(float x, float y) : pos({ x, y, 0.0f }) {}
};

ID3D11VertexShader* vertexShader;
ID3D11PixelShader* pixelShader;
ID3D11InputLayout* inputLayout;
ID3D11Buffer* vertexBuffer;


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;                                      // 윈도우 핸들

WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

//////////////////////////////////////////////////////////////////////////////////////////
//directX

ID3D11Device* device;               //CPU 접근(메모리를 비롯한 출력 외
ID3D11DeviceContext* deviceContext; //GPU 접근(출력)

IDXGISwapChain* swapChain;                  //DX내부적으로 돌아가는 후면버퍼 관리(=더블 버퍼)
ID3D11RenderTargetView* renderTargetView;   //VRAM 후면버퍼 메모리 관리

void InitDevice();
void Render();
void ReleaseDevice();

//////////////////////////////////////////////////////////////////////////////////////////

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECTX2D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTX2D));

    MSG msg = {};

    InitDevice();
    // 기본 메시지 루프입니다:
    while(msg.message != WM_QUIT) //while (GetMessage(&msg, nullptr, 0, 0))는 너무 느림
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            //메세지 있을 때에만 true
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else {
            //게임 루프
            Render();
        }
    }

    ReleaseDevice();
    return (int) msg.wParam;
}

void InitDevice()
{
    UINT width = WIN_WIDTH;
    UINT height = WIN_HEIGHT;

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

    //버퍼 정보
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
        D3D10_CREATE_DEVICE_DEBUG,  
        nullptr,                            //사양 체크
        0,                                  
        D3D11_SDK_VERSION,                  //개발 도구 버전
        &swapChainDesc,                     //위에서 넣은 정보들
        &swapChain,                         
        &device,
        nullptr,
        &deviceContext
    );

    ID3D11Texture2D* backBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();

    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

    /////////////////////////////////////////////////////////////////////////////////
    //뷰포트 설정
    D3D11_VIEWPORT viewport;    
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 0.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    deviceContext->RSSetViewports(1, &viewport);

    //셰이더 로드
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* vertexBlob;
    D3DCompileFromFile(L"Shaders/Tutorial.hlsl", nullptr, nullptr, "VS", "vs_5_0", flags, 0, &vertexBlob, nullptr);
    device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, &vertexShader);
}

void Render()
{
    //디바이스 컨텍스
    float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor); //렌더타겟뷰 초기화

    //Render();

    //백버퍼 스왑
    swapChain->Present(0, 0);
}

void ReleaseDevice()
{
    device->Release();
    deviceContext->Release();
    swapChain->Release();
    renderTargetView->Release();
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTX2D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECTX2D);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   //디바이스 크기 설정
   RECT rc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

   //전역변수
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        WIN_START_X, WIN_START_Y, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   //메뉴 빼기
   SetMenu(hWnd, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
