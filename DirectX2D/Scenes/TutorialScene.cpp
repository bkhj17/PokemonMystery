#include "Framework.h"
#include "TutorialScene.h"
#include "Objects/Polygon.h"

TutorialScene::TutorialScene()
{
	polygons.resize(30);
	for (auto& polygon : polygons)
		polygon = new SpawnPolygon();

}

TutorialScene::~TutorialScene()
{
	for(auto& polygon : polygons)
		delete polygon;
}

void TutorialScene::Update()
{
	SpawnRect();

	for (auto& polygon : polygons)
		polygon->Update();

}

void TutorialScene::Render()
{
	for (auto& polygon : polygons)
		polygon->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::SpawnRect()
{
	spawnTime += DELTA;
	if (spawnTime >= spawnRate) {
		for (auto polygon : polygons) {
			if (!polygon->isActive) {
				polygon->Spawn();
				spawnTime = 0.0f;
				spawnRate = Random(0.0f, 1.0f);
				break;
			}
		}
		
	}

}
