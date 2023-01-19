#include "Framework.h"

Effect::Effect(wstring textureFile, Vector2 maxFrame, float speed, bool isAdditive)
	: Quad(textureFile, Vector2(), Vector2(1.0f / maxFrame.x, 1.0f / maxFrame.y)),
	speed(speed), isAdditive(isAdditive)
{
	pixelShader = Shader::AddPS(L"PixelEffect.hlsl");

	isActive = false;

	frameBuffer = new FrameBuffer();
	frameBuffer->SetMaxFrame(maxFrame);

	maxFrameNum = (UINT)(maxFrame.x * maxFrame.y);
	maxFrameX = (UINT)(maxFrame.x);

	GetColor() = { 1.0f, 1.0f, 1.0f, 1.0f };
}

Effect::~Effect()
{
	delete frameBuffer;
}

void Effect::Update()
{
	if (!isActive)
		return;

	frameTime += speed * DELTA;

	if (frameTime >= delayTime)
	{
		frameTime -= delayTime;

		if (++curFrameNum >= maxFrameNum) {
			End();
		}

		frameBuffer->GetCurFrame().x = (float)(curFrameNum % maxFrameX);
		frameBuffer->GetCurFrame().y = (float)(curFrameNum / maxFrameX);
		float y = frameBuffer->GetCurFrame().y;
	}

	UpdateWorld();
}

void Effect::Render()
{
	if (!isActive) 
		return;

	if (isAdditive) {
		Environment::Get()->SetAdditiveBlend();
	}

	frameBuffer->SetPS(1);
	Quad::Render();

	Environment::Get()->SetAlphaBlend();

}

void Effect::Play(Vector2 position)
{
	isActive = true;
	Pos() = position;

	frameTime = 0.0f;

	curFrameNum = 0;

	frameBuffer->GetCurFrame() = { 0.0f, 0.0f };
}

void Effect::End()
{
	isActive = false;
}
