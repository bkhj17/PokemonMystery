#include "Framework.h"
#include "UIWindow.h"

UIWindow::UIWindow(Vector2 size, Vector2 pos)
	: Quad(size)
{
	isActive = false;
	localPosition = pos;
	SetShaderInfo();
}

UIWindow::UIWindow(float left, float right, float up, float down)
	: Quad(Vector2(right - left, up - down))
{
	localPosition = { (right + left) / 2, (up + down) / 2 };
	SetShaderInfo();
}

UIWindow::~UIWindow()
{
	delete floatBuffer;
}

void UIWindow::Init()
{
	isActive = true;
}

void UIWindow::Update()
{
	UpdateWorld();
}

void UIWindow::PostRender()
{
	colorBuffer->SetPS(0);
	floatBuffer->SetPS(1);

	__super::PostRender();
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
