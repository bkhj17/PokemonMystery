#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
	rects.resize(30);
	for (auto& rect : rects) {
		rect = new Rect({ 0,0 }, { 0.5f, 0.5f });
	}
}

TutorialScene::~TutorialScene()
{
	for(auto& rect : rects)
		delete rect;
}

void TutorialScene::Update()
{
	SpawnRect();

	for (auto& rect : rects)
		rect->Update();

}

void TutorialScene::Render()
{
	for (auto& rect : rects)
		rect->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::SpawnRect()
{
	spawnTime += DELTA;
	if (spawnTime >= spawnRate) {
		Rect* popRect = nullptr;
		for (auto rect : rects) {
			if (!rect->isActive) {
				popRect = rect;
				break;
			}
		}
		
		if (popRect != nullptr) {
			popRect->Spawn({ Random(-1.0f, 1.0f), Random(-1.0f, 1.0f) }, { Random(0.2f, 2.0f), Random(0.2f, 2.0f) });
			spawnTime = 0.0f;
			spawnRate = Random(0.0f, 1.0f);
		}
	}

}
