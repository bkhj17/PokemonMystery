#include "Framework.h"
#include "HPProgressBar.h"

HPProgressBar::HPProgressBar(wstring frontImageFile, wstring backImageFile)
	: ProgressBar(frontImageFile, backImageFile)
{
	SetPixelShader(L"PokemonProgressBar.hlsl");
	valueBuffer->Get()[1] = renderAmount;
}

HPProgressBar::~HPProgressBar()
{
}

void HPProgressBar::SetRenderAmount(float renderAmount)
{
	valueBuffer->Get()[1] = renderAmount;
}