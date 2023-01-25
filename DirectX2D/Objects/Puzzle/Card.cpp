#include "Framework.h"
#include "Card.h"


Card::Card(wstring textureFile)
	: Button(textureFile), key(textureFile)
{
	SetEvent(bind(&Card::OnClick, this));

	pixelShader = Shader::AddPS(L"PixelOutline.hlsl");

	intValueBuffer = new IntValueBuffer;
	floatValueBuffer = new FloatValueBuffer;
	outlineColorBuffer = new ColorBuffer;

	floatValueBuffer->Get()[0] = size.x;
	floatValueBuffer->Get()[1] = size.y;

	outlineColorBuffer->Get() = YELLOW;
}

Card::~Card()
{
	delete intValueBuffer;
	delete floatValueBuffer;
	delete outlineColorBuffer;
}

void Card::Update()
{
	if (!isActive)
		return;

	intValueBuffer->Get()[0] = isSelected ? 1 : 0;

	__super::Update();
}

void Card::Render()
{
	if (!isActive)
		return;

	intValueBuffer->SetPS(1);
	floatValueBuffer->SetPS(2);
	outlineColorBuffer->SetPS(3);

	Quad::Render();
}

void Card::OnClick()
{
	Observer::Get()->ExecuteParamEvent("ClickCard", this);
}
