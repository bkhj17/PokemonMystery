#include "Framework.h"
#include "UIWindow.h"

UIWindow::UIWindow(Vector2 size, Vector2 pos)
	: Quad(size)
{
	isActive = false;
	localPosition = pos;
	SetShaderInfo();

	CreateCursor();
}

UIWindow::UIWindow(float left, float right, float up, float down)
	: Quad(Vector2(right - left, up - down))
{
	localPosition = { (right + left) / 2, (up + down) / 2 };
	UpdateWorld();
	SetShaderInfo();

	CreateCursor();
}

UIWindow::~UIWindow()
{
	delete floatBuffer;
	delete cQuad;
}

void UIWindow::Init()
{
	isActive = true;
	cursor = 0;
}

void UIWindow::Update()
{
	if (KEY_DOWN(VK_DOWN))
		cursor = (++cursor) % maxCursor;

	if (KEY_DOWN(VK_UP))
		cursor = cursor == 0 ? maxCursor - 1 : cursor-1;

	if (KEY_DOWN('X'))
		Close();

	UpdateWorld();
}

void UIWindow::RenderCursor()
{
	cQuad->PostRender();
}

void UIWindow::PostRender()
{
	colorBuffer->SetPS(0);
	floatBuffer->SetPS(1);

	__super::PostRender();
}

void UIWindow::Close()
{
	isActive = false;
}

void UIWindow::SetShaderInfo()
{
	SetTexture(Texture::Add(L"Textures/pokemon/UI/Blueframe.png"));
	floatBuffer = new FloatValueBuffer;

	SetPixelShader(L"WindowFrame.hlsl");
	//
	colorBuffer->Get() = { 0.062f, 0.140f, 0.203f, 1.0f };
	floatBuffer->Get()[0] = 0.05f;
	floatBuffer->Get()[1] = 0.05f;
	floatBuffer->Get()[2] = 0.8f;
}

void UIWindow::CreateCursor()
{
	cQuad = new Quad(Vector2(50.0f, 50.0f));
	cQuad->SetTexture(L"Textures/pokemon/UI/Cursor.png");
	cQuad->SetParent(this);
}
