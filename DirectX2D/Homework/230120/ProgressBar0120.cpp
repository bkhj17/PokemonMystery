#include "Framework.h"
#include "ProgressBar0120.h"

ProgressBar0120::ProgressBar0120(wstring textureFile)
	: Quad(textureFile)
{
	floatValueBuffer = new FloatValueBuffer;
	colorBuffer->Get() = { 1.0f, 0.0f, 0.0f, 1.0f };

	pixelShader = Shader::AddPS(L"PixelProgress0120.hlsl");
}

ProgressBar0120::~ProgressBar0120()
{
	delete floatValueBuffer;
}

void ProgressBar0120::Render()
{
	floatValueBuffer->Get()[0] = Clamp(0.0f, 1.0f, curValue * invMaxValue);
	floatValueBuffer->SetPS(1);

		
	__super::Render();
}

void ProgressBar0120::Init(float maxValue)
{
	curValue = this->maxValue = maxValue;
	invMaxValue = 1.0f / maxValue;
}
