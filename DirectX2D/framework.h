// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

//사용자 정의 상수
#define WIN_WIDTH	1280
#define WIN_HEIGHT	720

#define WIN_START_X 100
#define WIN_START_Y 100

#define CENTER_X	(WIN_WIDTH * 0.5f)
#define CENTER_Y	(WIN_HEIGHT * 0.5f)

#define RED { 1.0f, 0.0f, 0.0f, 1.0f }
#define GREEN { 0.0f, 1.0f, 0.0f, 1.0f }
#define BLUE { 0.0f, 0.0f, 1.0f, 1.0f }
#define YELLOW { 1.0f, 1.0f, 0.0f, 1.0f }
#define CYAN { 0.0f, 1.0f, 1.0f, 1.0f }
#define MAGENTA { 1.0f, 0.0f, 0.0f, 1.0f }
#define BLACK { 0.0f, 0.0f, 0.0f, 1.0f }
#define WHITE {1.0f, 1.0f, 1.0f, 1.0f }

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define DELTA Timer::Get()->GetElapsedTime()

#define KEY_DOWN(k) KeyBoard::Get()->Down(k)
#define KEY_UP(k) KeyBoard::Get()->Up(k)
#define KEY_PRESS(k) KeyBoard::Get()->Press(k)

// Windows 헤더 파일
#include <windows.h>
/*
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
*/
// c 헤더
#include <cmath>
#include <cstring>

// c++ 헤더
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <functional>
using namespace std;

#include <d3d11.h>
#include <DirectXMath.h>
//셰이더 관련 라이브러리
#include <d3dcompiler.h>

//정적 라이브러리 포함
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <DirectXTex/DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")
using namespace DirectX;

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

#pragma comment(lib, "ImGui.lib")

//FMOD Library
#include "Libraries/inc/fmod.hpp"
#pragma comment(lib, "Libraries/lib/fmod_vc.lib")


typedef DirectX::XMFLOAT4 Float4;
typedef DirectX::XMFLOAT3 Float3;
typedef DirectX::XMFLOAT2 Float2;
typedef DirectX::XMMATRIX Matrix;

//Framework Header
#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/KeyBoard.h"
#include "Framework/Utilities/Audio.h"
#include "Framework/Utilities/Utility.h"


#include "Framework/Math/Vector2.h"
#include "Framework/Math/GameMath.h"
#include "Framework/Math/Transform.h"
using namespace GameMath;

#include "Framework/System/Device.h"

#include "Framework/Shaders/Shader.h"
#include "Framework/Shaders/VertexShader.h"
#include "Framework/Shaders/PixelShader.h"

#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/VertexLayouts.h"
#include "Framework/Buffer/GlobalBuffer.h"

#include "Framework/Render/Texture.h"

#include "Framework/Environment/Environment.h"

#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Quad.h"

#include "Objects/Shooting/Plane.h"
#include "Objects/Shooting/Bullet.h"
#include "Objects/Shooting/BulletManager.h"

#include "Scenes/Scene.h"

#include "Manager/GameManager.h"

extern HWND hWnd;
extern Vector2 mousePos;
