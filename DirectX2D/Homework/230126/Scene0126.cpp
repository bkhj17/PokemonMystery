#include "Framework.h"
#include "Scene0126.h"
#include "TileMap0126.h"
#include "EnemyTank0126.h"

Scene0126::Scene0126()
{
	gameTileMap = new TileMap0126("TextData/TestMap.map");
	gameTileMap->SetLoseEvent(bind(&Scene0126::GameOver, this));

	tank = new Tank();
	tank->Pos() = gameTileMap->PlayerSpawnPoint();
	tank->SetDestroyEvent(bind(&Scene0126::BreakPlayer, this));

	enemies.resize(MAX_SPAWN_ENEMY);
	for (auto& enemy : enemies) {
		enemy = new EnemyTank0126;
		enemy->SetDestroyEvent(bind(&Scene0126::BreakEnemy, this));
		enemy->SetTileMap(gameTileMap);
	}
	enemyNum = 15;

	BulletManager::Get();

	gameOver = new Quad(L"Textures/Shooting/GameOver.png");
	gameOver->Pos() = { CENTER_X, CENTER_Y };
	gameOver->UpdateWorld();
	gameClear = new Quad(L"Textures/Shooting/GameClear.png");
	gameClear->Pos() = { CENTER_X, CENTER_Y };
	gameClear->UpdateWorld();
}

Scene0126::~Scene0126()
{
	delete gameTileMap;
	delete tank;
	for (auto enemy : enemies)
		delete enemy;

	delete gameOver;
	delete gameClear;
	BulletManager::Delete();
}

void Scene0126::Update()
{
	if (state == PLAY)
		UpdatePlay();
}

void Scene0126::Render()
{
	gameTileMap->Render();
	tank->Render();

	BulletManager::Get()->Render();

	for (auto enemy : enemies)
		enemy->Render();
}

void Scene0126::PostRender()
{
	tank->PostRender();

	switch (state)
	{
	case Scene0126::PLAY:
		break;
	case Scene0126::CLEAR:
		gameClear->Render();
		break;
	case Scene0126::GAME_OVER:
		gameOver->Render();
		break;
	default:
		break;
	}
}

void Scene0126::UpdatePlay()
{

	EnemySpawn();
	for (auto enemy : enemies) {
		enemy->Update();
		gameTileMap->PushObject(enemy->GetCollider());
	}

	PlayerSpawn();
	tank->Update();
	if (KEY_DOWN(VK_SPACE))
		tank->Fire();

	gameTileMap->PushObject(tank->GetCollider());
	gameTileMap->BulletCollision();

	BulletManager::Get()->Update();
}

void Scene0126::EnemySpawn()
{
	enemySpawnTime -= DELTA;

	if (enemySpawnTime < min(MAX_SPAWN_ENEMY, enemyNum) && enemySpawnTime < 0.0f) {
		for (auto enemy : enemies) {
			if (!enemy->Active()) {
				spawnEnemy++;
				enemySpawnTime = spawnRate;
				enemy->Spawn(gameTileMap->EnemySpawnPoint());
				break;
			}
		}
	}
}

void Scene0126::PlayerSpawn()
{
	if (!tank->Active()) {
		playerSpawnTime -= DELTA;
		if (playerSpawnTime < 0.0f) {
			tank->SetActive(true);
			tank->Pos() = gameTileMap->PlayerSpawnPoint();
		}
	}
}

void Scene0126::BreakEnemy()
{
	--spawnEnemy;
	if (--enemyNum <= 0)
		Win();
}

void Scene0126::BreakPlayer()
{
	playerSpawnTime = spawnRate;
}

void Scene0126::Win()
{
	state = CLEAR;
}

void Scene0126::GameOver()
{
	state = GAME_OVER;
}
