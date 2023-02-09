#include "Framework.h"

ProgressBar::ProgressBar(wstring frontImageFile, wstring backImageFile)
    : Quad(frontImageFile)
{
    SetPixelShader(L"ProgressBar.hlsl");

    backImage = Texture::Add(backImageFile);

    valueBuffer = new FloatValueBuffer();
    valueBuffer->Get()[0] = fillAmount;
}

ProgressBar::~ProgressBar()
{
    delete valueBuffer;
}

void ProgressBar::Render()
{
    valueBuffer->SetPS(1);
    backImage->PSSet(1);

    Quad::Render();
}

void ProgressBar::PostRender()
{
    pixelShader->Set();

    valueBuffer->SetPS(1);
    backImage->PSSet(1);

    Quad::PostRender();
}

void ProgressBar::SetAmount(float value)
{
    valueBuffer->Get()[0] = value;
}
