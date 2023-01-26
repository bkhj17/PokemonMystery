#include "Framework.h"
#include "RenderTargetScene.h"

RenderTargetScene::RenderTargetScene()
{
	bg = new Quad(L"Textures/Shooting/background.png");
	bg->Pos() = { CENTER_X, CENTER_Y };
	bg->Scale() *= 6.0f;
	bg->UpdateWorld();

	plane = new Plane;
	plane->Pos() = { CENTER_X, CENTER_Y };

	ninja = new Ninja;
	ninja->Pos() = { CENTER_X * 0.5f, CENTER_Y };

	renderTarget = new RenderTarget(CENTER_X, WIN_HEIGHT);
	Texture* texture = Texture::Add(L"rt", renderTarget->GetSRV());

	Quad* renderTexture = new Quad(Vector2(CENTER_X, WIN_HEIGHT));
	renderTexture->Pos() = { CENTER_X / 2, CENTER_Y };
	renderTexture->UpdateWorld();
	renderTexture->SetTexture(texture);
	renderTextures.push_back(renderTexture);

	ninjaRenderTarget = new RenderTarget(CENTER_X, WIN_HEIGHT);
	texture = Texture::Add(L"nrt", ninjaRenderTarget->GetSRV());

	renderTexture = new Quad(Vector2(CENTER_X, WIN_HEIGHT));
	renderTexture->Pos() = { CENTER_X / 2 * 3, CENTER_Y};
	renderTexture->UpdateWorld();
	renderTexture->SetTexture(texture);
	renderTexture->SetPixelShader(L"Light.hlsl");
	renderTextures.push_back(renderTexture);

	intValueBuffer = new IntValueBuffer();
	intValueBuffer->Get()[0] = 1;

	floatValueBuffer = new FloatValueBuffer;
	floatValueBuffer->Get()[0] = CENTER_X;
	floatValueBuffer->Get()[1] = WIN_HEIGHT;

	lightBuffer = new FloatValueBuffer;

	CAM->SetTarget(plane);
	CAM->SetOffset(CENTER_X / 2, CENTER_Y);
	Vector2 leftBottom = bg->Pos() - bg->Half() * bg->Scale();
	Vector2 rightTop = bg->Pos() + bg->Half() * bg->Scale();
	CAM->SetLeftBottom(leftBottom.x, leftBottom.y);
	CAM->SetRightTop(rightTop.x, rightTop.y);
	CAM->SetDeviceSize(CENTER_X, WIN_HEIGHT);

	ninjaCamera = new Camera();
	ninjaCamera->SetTarget(ninja);

	ninjaCamera->SetOffset(CENTER_X / 2, CENTER_Y);
	leftBottom = bg->Pos() - bg->Half() * bg->Scale();
	rightTop = bg->Pos() + bg->Half() * bg->Scale();
	ninjaCamera->SetLeftBottom(leftBottom.x, leftBottom.y);
	ninjaCamera->SetRightTop(rightTop.x, rightTop.y);
	ninjaCamera->SetDeviceSize(CENTER_X, WIN_HEIGHT);

}

RenderTargetScene::~RenderTargetScene()
{
	delete bg;
	delete plane;
	delete ninja;

	delete renderTarget;
	delete ninjaRenderTarget;

	for (auto renderTexture : renderTextures)
		delete renderTexture;
	delete intValueBuffer;
	delete floatValueBuffer;
	delete lightBuffer;
}

void RenderTargetScene::Update()
{
	plane->Update();
	ninja->Update();

	ninjaCamera->Update();

	Vector2 planePos = ninjaCamera->WorldToScreen(plane->Pos());
	Vector2 ninjaPos = ninjaCamera->WorldToScreen(ninja->Pos());

	lightBuffer->Get()[0] = planePos.x;
	lightBuffer->Get()[1] = planePos.y;
	lightBuffer->Get()[2] = ninjaPos.x;
	lightBuffer->Get()[3] = ninjaPos.y;
}

void RenderTargetScene::PreRender()
{
	renderTarget->Set();
	CAM->SetView();

	bg->Render();
	plane->Render();
	ninja->Render();

	ninjaRenderTarget->Set();
	ninjaCamera->SetView();

	bg->Render();
	plane->Render();
	ninja->Render();
}

void RenderTargetScene::Render()
{
}

void RenderTargetScene::PostRender()
{
	intValueBuffer->SetPS(1);
	floatValueBuffer->SetPS(2);
	lightBuffer->SetPS(3);

	for (auto renderTexture : renderTextures)
		renderTexture->Render();

	ImGui::SliderInt("Index", intValueBuffer->Get(), 0, 20);
	ImGui::SliderInt("Range", &intValueBuffer->Get()[1], 0, 1000);
	ImGui::SliderInt("Squared", &intValueBuffer->Get()[2], 0, 10);
}
