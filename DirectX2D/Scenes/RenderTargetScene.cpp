#include "Framework.h"
#include "RenderTargetScene.h"

RenderTargetScene::RenderTargetScene()
{
	bg = new Quad(L"Textures/Shooting/background.png");
	bg->Pos() = { CENTER_X, CENTER_Y };
	bg->Scale() *= 3.0f;
	bg->UpdateWorld();

	plane = new Plane;
	plane->Pos() = { CENTER_X, CENTER_Y };

	renderTarget = new RenderTarget;
	Texture* texture = Texture::Add(L"rt", renderTarget->GetSRV());
	Quad* renderTexture;
	renderTexture = new Quad(Vector2(CENTER_X, CENTER_Y));
	renderTexture->Pos() = { CENTER_X / 2, CENTER_Y / 2 };
	renderTexture->UpdateWorld();
	renderTexture->SetTexture(texture);
	renderTextures.push_back(renderTexture);

	renderTexture = new Quad(Vector2(CENTER_X, CENTER_Y));
	renderTexture->Pos() = { CENTER_X / 2 * 3, CENTER_Y / 2 };
	renderTexture->UpdateWorld();
	renderTexture->SetTexture(texture);
	renderTexture->SetPixelShader(L"Filter.hlsl");
	renderTextures.push_back(renderTexture);

	intValueBuffer = new IntValueBuffer();
	intValueBuffer->Get()[0] = 1;

	floatValueBuffer = new FloatValueBuffer;
	floatValueBuffer->Get()[0] = WIN_WIDTH;
	floatValueBuffer->Get()[1] = WIN_HEIGHT;
}

RenderTargetScene::~RenderTargetScene()
{
	delete bg;
	delete plane;
	delete renderTarget;

	for (auto renderTexture : renderTextures)
		delete renderTexture;
	delete intValueBuffer;
	delete floatValueBuffer;
}

void RenderTargetScene::Update()
{
	plane->Update();
}

void RenderTargetScene::PreRender()
{
	renderTarget->Set();
	bg->Render();
	plane->Render();
}

void RenderTargetScene::Render()
{
}

void RenderTargetScene::PostRender()
{
	intValueBuffer->SetPS(1);
	floatValueBuffer->SetPS(2);

	for (auto renderTexture : renderTextures)
		renderTexture->Render();
}
