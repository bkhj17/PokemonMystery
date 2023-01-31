#include "Framework.h"
#include "Scene0130.h"
#include "TileMap0130.h"
#include "Pacman0130.h"
#include "Enemy0130.h"

Scene0130::Scene0130()
{
	tileMap = new TileMap0130("TextData/TestMap2.map");
	tileMap->Scale() *= 0.45f;
	tileMap->Pos() = { 15.0f, 15.0f };
	tileMap->UpdateWorld();
	tileMap->SetWinEvent(bind(&Scene0130::SetGameState, this, CLEAR));

	pacman = new Pacman0130();
	pacman->SetPos({ 130.0f, 160.0f });
	pacman->Scale() *= 25.0f;
	pacman->SetMoveLength(tileMap->GetTileSize().x);

	tileMap->SetPushEvent([this]() {
		pacman->SetPos(pacman->Pos());
		});

	gameOver = new Quad(L"Textures/Shooting/GameOver.png");
	gameOver->Pos() = { CENTER_X, CENTER_Y };
	gameOver->UpdateWorld();
	gameClear = new Quad(L"Textures/Shooting/GameClear.png");
	gameClear->Pos() = { CENTER_X, CENTER_Y };
	gameClear->UpdateWorld();

	astar = new AStar(tileMap);

	enemies.resize(4);
	for (auto& enemy : enemies) {
		enemy = new Enemy0130;
		enemy->Pos() = { 350.0f, 350.0f };
		enemy->Scale() *= 25.0f;
		enemy->UpdateWorld();
	}

	Observer::Get()->AddParamEvent("GetPlayerPos", bind(&Scene0130::GetPlayerPos, this, placeholders::_1));
	Observer::Get()->AddParamEvent("EnemyCallPath", bind(&Scene0130::EnemyCallPath, this, placeholders::_1));
}

Scene0130::~Scene0130()
{
	delete tileMap;
	delete pacman;

	delete gameOver;
	delete gameClear;

	delete astar;
	for (auto enemy : enemies)
		delete enemy;

}

void Scene0130::Update()
{
	if (state == PLAY) {
		pacman->Update();
		tileMap->PushObject(pacman->GetCollider());
		tileMap->ItemCollision(pacman->GetCollider());
	}

	for (auto enemy : enemies) {
		if (enemy->Active()) {
			Vector2 vec = pacman->Pos() - enemy->Pos();
			if (pacman->Active() && vec.Length() < 250.0f) {
				enemy->SetAction(Enemy0130::CHASE);
			}
			else
				enemy->SetAction(Enemy0130::PATROL);

			enemy->Update();

			if (enemy->GetCollider()->IsCollision(pacman->GetCollider())) {
				if (pacman->IsSuper()) {
					enemy->Destroy();
				}
				else {
					pacman->Dead();
					SetGameState(GAME_OVER);
				}
			}
		}
	}
}

void Scene0130::Render()
{
	tileMap->Render();
	pacman->Render();

	for (auto enemy : enemies) {
		if (enemy->Active())
			enemy->Render();
	}
}

void Scene0130::PostRender()
{
	switch (state)
	{
	case Scene0130::PLAY:
		break;
	case Scene0130::CLEAR:
		gameClear->Render();
		break;
	case Scene0130::GAME_OVER:
		gameOver->Render();
		break;
	default:
		break;
	}
}

void Scene0130::EnemyCallPath(void* caller)
{
	auto enemy = (Enemy0130*)caller;
	if (enemy == nullptr)
		return;

	int curNode = astar->FindCloseNode(enemy->Pos());
	int targetNode = astar->FindCloseNode(enemy->GetTargetPos());
	if (targetNode == -1)
		return;
	astar->GetPath(curNode, targetNode, enemy->GetPath());
}

void Scene0130::GetPlayerPos(void* pos)
{
	auto p = (Vector2*)pos;
	*p = pacman->Pos();
}