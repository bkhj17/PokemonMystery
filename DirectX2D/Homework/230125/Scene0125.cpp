#include "Framework.h"
#include "Scene0125.h"

Scene0125::Scene0125()
{
	bg = new Quad(L"Textures/Shooting/background.png");
	bg->Scale() *= 20.0f;
	bg->UpdateWorld();

	plane1 = new Plane();
	plane1->SetTag("Plane1");
	plane1->Pos() = { -bg->Half().x * bg->Scale().x * 0.5f, 0.0f};
	plane1->Update();

	cam1 = new Camera;
	cam1->SetTarget(plane1);
	renderTarget = new RenderTarget;

	plane2 = new Plane();
	plane2->SetTag("Plane2");
	plane2->Pos() = { bg->Half().x * bg->Scale().x * 0.5f, 0.0f };
	plane2->Update();

	plane2->upKey = VK_UP;
	plane2->downKey = VK_DOWN;
	plane2->leftKey = VK_LEFT;
	plane2->rightKey = VK_RIGHT;

	cam2 = new Camera;
	cam2->SetTarget(plane2);

	renderTarget2 = new RenderTarget;
	Texture* texture = Texture::Add(L"rt0", renderTarget->GetSRV());
	Quad* renderTexture = new Quad(Vector2(CENTER_X, CENTER_Y));
	renderTexture->Pos() = { CENTER_X / 2, CENTER_Y };
	renderTexture->UpdateWorld();
	renderTexture->SetTexture(texture);
	renderTexture->SetPixelShader(L"Light.hlsl");
	renderTextures.push_back(renderTexture);

	texture = Texture::Add(L"rt1", renderTarget2->GetSRV());
	renderTexture = new Quad(Vector2(CENTER_X, CENTER_Y));
	renderTexture->Pos() = { CENTER_X / 2 * 3, CENTER_Y};
	renderTexture->UpdateWorld();
	renderTexture->SetTexture(texture);
	renderTexture->SetPixelShader(L"Light.hlsl");
	renderTextures.push_back(renderTexture);

	//셰이더 버퍼
	intValueBuffer = new IntValueBuffer;
	intValueBuffer->Get()[0] = 2;
	intValueBuffer->Get()[1] = 200;
	intValueBuffer->SetPS(1);

	sizeValueBuffer = new FloatValueBuffer;
	sizeValueBuffer->Get()[0] = WIN_WIDTH;
	sizeValueBuffer->Get()[1] = WIN_HEIGHT;
	sizeValueBuffer->SetPS(2);

	posValueBuffer = new FloatValueBuffer;
}

Scene0125::~Scene0125()
{
	delete bg;
	delete plane1;
	delete cam1;
	delete renderTarget;

	delete plane2;
	delete cam2;
	delete renderTarget2;

	for (auto renderTexture : renderTextures)
		delete renderTexture;

	delete intValueBuffer;
	delete sizeValueBuffer;
	delete posValueBuffer;
}

void Scene0125::Update()
{
	Vector2 bgHalfSize = bg->Half() * bg->Scale();
	Vector2 bgLB = bg->GlobalPos() - bgHalfSize;
	Vector2 bgRT = bg->GlobalPos() + bgHalfSize;

	plane1->Update();
	cam1->Update();
	//CAM Border
	cam1->Pos().x = max(cam1->Pos().x, bgLB.x);
	cam1->Pos().x = min(cam1->Pos().x, bgRT.x - WIN_WIDTH);
	cam1->Pos().y = max(cam1->Pos().y, bgLB.y);
	cam1->Pos().y = min(cam1->Pos().y, bgRT.y - WIN_HEIGHT);
	cam1->UpdateWorld();

	plane2->Update();
	cam2->Update();
	//CAM Border
	cam2->Pos().x = max(cam2->Pos().x, bgLB.x);
	cam2->Pos().x = min(cam2->Pos().x, bgRT.x - WIN_WIDTH);
	cam2->Pos().y = max(cam2->Pos().y, bgLB.y);
	cam2->Pos().y = min(cam2->Pos().y, bgRT.y - WIN_HEIGHT);
	cam2->UpdateWorld();
}

void Scene0125::PreRender()
{

	//plane1 시점
	renderTarget->Set();
	cam1->SetView();
	bg->Render();
	plane1->Render();
	plane2->Render();

	//plane2 시점
	renderTarget2->Set();
	cam2->SetView();

	bg->Render();
	plane1->Render();
	plane2->Render();
}

void Scene0125::Render()
{
	posValueBuffer->Get()[0] = plane1->Pos().x - cam1->Pos().x;
	posValueBuffer->Get()[1] = WIN_HEIGHT - (plane1->Pos().y - cam1->Pos().y);
	posValueBuffer->Get()[2] = plane2->Pos().x - cam1->Pos().x;
	posValueBuffer->Get()[3] = WIN_HEIGHT - (plane2->Pos().y - cam1->Pos().y);
	posValueBuffer->SetPS(3);
	renderTextures[0]->Render();

	posValueBuffer->Get()[0] = plane1->Pos().x - cam2->Pos().x;
	posValueBuffer->Get()[1] = WIN_HEIGHT - (plane1->Pos().y - cam2->Pos().y);
	posValueBuffer->Get()[2] = plane2->Pos().x - cam2->Pos().x;
	posValueBuffer->Get()[3] = WIN_HEIGHT - (plane2->Pos().y - cam2->Pos().y);
	posValueBuffer->SetPS(3);
	renderTextures[1]->Render();
}

void Scene0125::PostRender()
{
	plane1->RenderUI();
	plane2->RenderUI();
}
