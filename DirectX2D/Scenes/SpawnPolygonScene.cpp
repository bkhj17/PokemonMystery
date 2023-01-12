#include "Framework.h"
#include "SpawnPolygonScene.h"

SpawnPolygonScene::SpawnPolygonScene()
{
	polygons.resize(10);
	for (auto& polygon : polygons)
		polygon = new SpawnPolygon();
}

SpawnPolygonScene::~SpawnPolygonScene()
{
	for (auto polygon : polygons)
		delete polygon;
}

void SpawnPolygonScene::Update()
{
	spawnTime -= DELTA;
	if (spawnTime < 0.0f) {
		spawnTime = 0.1f;
		Spawn();
	}

	for (auto polygon : polygons)
		polygon->Update();

}

void SpawnPolygonScene::Render()
{
	for (auto polygon : polygons)
		polygon->Render();
}

void SpawnPolygonScene::PostRender()
{
}

void SpawnPolygonScene::Spawn()
{
	for (auto polygon : polygons) {
		if (!polygon->isActive) {
			polygon->Spawn();
			return;
		}
	}
}
