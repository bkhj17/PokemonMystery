#include "Framework.h"
#include "SichuanCard0120.h"

SichuanCard0120::SichuanCard0120(Vector2 size)
	: Button(size)
{
	isActive = false;
}

void SichuanCard0120::Init(int index, Vector2 pos, wstring file)
{
	isActive = true;
	this->index = index;
	SetTexture(file);
	localPosition = pos;
}
