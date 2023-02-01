#include "Framework.h"
#include "InstancingScene.h"

InstancingScene::InstancingScene()
{
	/*
	quads.resize(SIZE);

	for (auto& quad : quads) {
		quad = new Quad(L"Textures/Cards/Card (1).png");
		quad->Pos().x = Random(0, WIN_WIDTH);
		quad->Pos().y = Random(0, WIN_HEIGHT);
		quad->Scale().x = Random(1.0f, 3.0f);
		quad->Scale().y = Random(1.0f, 3.0f);
		quad->UpdateWorld();
	}
	*/
	quad = new Quad(L"Textures/Shooting/Isaac_Head.png", Vector2(), Vector2(0.25f, 0.33f));
	quad->SetVertexShader(L"Instancing.hlsl");
	quad->SetPixelShader(L"Instancing.hlsl");

	instances.resize(SIZE);
	for (auto& instance : instances) {
		Transform transform;
		transform.Pos().x = Random(0, WIN_WIDTH);
		transform.Pos().y = Random(0, WIN_HEIGHT);
		transform.Scale().x = Random(1.0f, 3.0f);
		transform.Scale().y = Random(1.0f, 3.0f);
		transform.UpdateWorld();

		instance.maxFrame = { 4, 3 };
		instance.curFrame.x = Random(0, 4);
		instance.curFrame.y = Random(0, 3);

		instance.transform = XMMatrixTranspose(transform.GetWorld());
	}

	instanceBuffer = new VertexBuffer(instances.data(), sizeof(InstanceData), SIZE);
}

InstancingScene::~InstancingScene()
{
	//for (auto quad : quads)
	//	delete quad;
	delete quad;
	delete instanceBuffer;
}

void InstancingScene::Update()
{
}

void InstancingScene::Render()
{
	instanceBuffer->Set(1);
	quad->SetRender();
	DC->DrawIndexedInstanced(6, SIZE, 0, 0, 0);
}

void InstancingScene::PostRender()
{
}
