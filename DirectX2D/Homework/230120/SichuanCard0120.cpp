#include "Framework.h"
#include "SichuanCard0120.h"

SichuanCard0120::SichuanCard0120(Vector2 size)
	: Button(size)
{
	isActive = false;

	pixelShader = Shader::AddPS(L"PixelNinja.hlsl");
	showTime = 0.0f;
}

void SichuanCard0120::Init(int index, Vector2 pos, wstring file)
{
	isActive = true;
	this->index = index;
	SetTexture(file);
	localPosition = pos;
}

void SichuanCard0120::Update()
{
	if (!isActive)
		return;

	if (showTime > 0.0f) {
		showTime -= DELTA;
		if (showTime <= 0.0f) {
			SetActive(false);
		}
	}
	else {
		__super::Update();
	}
}

void SichuanCard0120::Render()
{
	__super::Render();

	if(isActive && selected)
		GetCollider()->Render();
}

void SichuanCard0120::Kill()
{
	showTime = showRate;
	SetSelected(false);
}
