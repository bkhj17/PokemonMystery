// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

//사용자 정의 상수
#define WIN_WIDTH	1700
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

#define CAM Environment::Get()->GetMainCamera()

#define DIALOG ImGuiFileDialog::Instance()

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
#include <unordered_set>
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
#pragma comment(lib, "dxguid.lib")

//DirectXTex
#include <DirectXTex/DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")
using namespace DirectX;

//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <dirent/dirent.h>
#include <ImGuiFileDialog.h>
#include <ImGuiFileDialogConfig.h>

#pragma comment(lib, "ImGui.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//FMOD Library
#include "Libraries/inc/fmod.hpp"
#pragma comment(lib, "Libraries/lib/fmod_vc.lib")


typedef DirectX::XMFLOAT4 Float4;
typedef DirectX::XMFLOAT3 Float3;
typedef DirectX::XMFLOAT2 Float2;
typedef DirectX::XMMATRIX Matrix;

typedef function<void()> Event;
typedef function<void(void*)> ParamEvent;
typedef function<void(int)> IntParamEvent;

//Framework Header
#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/KeyBoard.h"
#include "Framework/Utilities/Audio.h"
#include "Framework/Utilities/Utility.h"
#include "Framework/Utilities/Observer.h"
#include "Framework/Utilities/tinyxml2.h"
#include "Framework/Utilities/Font.h"
#include "Framework/Utilities/BinaryWriter.h"
#include "Framework/Utilities/BinaryReader.h"
using namespace Utility;

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
#include "Framework/Render/Frame.h"
#include "Framework/Render/Clip.h"
#include "Framework/Render/RenderTarget.h"

#include "Framework/Collision/Collider.h"
#include "Framework/Collision/RectCollider.h"
#include "Framework/Collision/CircleCollider.h"
#include "Framework/Collision/LineCollider.h"

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Environment.h"

#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Quad.h"
#include "Objects/Basic/Action.h"
#include "Objects/Basic/Effect.h"

#include "Objects/UI/Button.h"

#include "Objects/Algorithm/DNode.h"
#include "Objects/Algorithm/Dijkstra.h"
#include "Objects/Algorithm/Node.h"
#include "Objects/Algorithm/Heap.h"
#include "Objects/Algorithm/AStar.h"

#include "Objects/Tile/Tile.h"
#include "Objects/Tile/EditTileMap.h"
#include "Objects/Tile/GameTileMap.h"
#include "Objects/Tile/Tank.h"

#include "Objects/Manager/EffectManager.h"
#include "Objects/Manager/CommandManager.h"

#include "Objects/Shooting/Plane.h"
#include "Objects/Shooting/Bullet.h"
#include "Objects/Shooting/BulletManager.h"

#include "Objects/PinBall/Bat.h"
#include "Objects/PinBall/Ball.h"

#include "Objects/Character/Ninja/NinjaIdle.h"
#include "Objects/Character/Ninja/NinjaRun.h"
#include "Objects/Character/Ninja/NinjaJump.h"
#include "Objects/Character/Ninja/NinjaJumpUp.h"
#include "Objects/Character/Ninja/NinjaJumpDown.h"
#include "Objects/Character/Ninja/NinjaGlide.h"
#include "Objects/Character/Ninja/NinjaAttack.h"
#include "Objects/Character/Ninja/NinjaMeleeAttack.h"
#include "Objects/Character/Ninja/NinjaJumpAttack.h"
#include "Objects/Character/Ninja/NinjaThrow.h"
#include "Objects/Character/Ninja/Ninja.h"

#include "Objects/Puzzle/Card.h"

#include "Scenes/Scene.h"

#include "Manager/GameManager.h"

extern HWND hWnd;
extern Vector2 mousePos;
extern float mouseWheel;
