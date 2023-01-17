#include "Framework.h"
#include "Scene0116.h"
#include "Area0116.h"
#include "Bar0116.h"
#include "Ball0116.h"
#include "Block0116.h" 

Scene0116::Scene0116()
{
	area = new Area0116({ 300.0f, WIN_HEIGHT });
	area->Pos() = { CENTER_X, CENTER_Y };
	area->UpdateWorld();


	ball = new Ball0116(20.0f);
	ball->SetTag("Ball");

	CreateBar();
	CreateBlocks();

	SetBall();
	SetBlocks();

	gameOverMessage = new Quad(L"Textures/Shooting/GameOver.png", { CENTER_X, CENTER_Y });
	gameClearMessage = new Quad(L"Textures/Shooting/GameClear.png", { CENTER_X, CENTER_Y });
}

Scene0116::~Scene0116()
{
	delete area;
	delete leftBar;
	delete rightBar;

	for (auto block : blocks)
		delete block;
}

void Scene0116::Update()
{
	switch (state)
	{
	case Scene0116::GameState::PLAY:
		Play();
		break;
	case Scene0116::GameState::CLEAR:
	case Scene0116::GameState::GAME_OVER:
		if (KEY_DOWN(VK_SPACE)) {
			SetBall();
			SetBlocks();
			state = GameState::PLAY;
		}
		break;
	}
}

void Scene0116::Render()
{
	area->Render();
	leftBar->Render();
	rightBar->Render();
	ball->Render();

	for (auto block : blocks)
		block->Render();
	
}

void Scene0116::PostRender()
{
	leftBar->RenderUI();
	rightBar->RenderUI();
	ball->RenderUI();


	switch (state)
	{
	case Scene0116::GameState::PLAY:
		break;
	case Scene0116::GameState::CLEAR:
		ClearRender();
		break;
	case Scene0116::GameState::GAME_OVER:
		GameOverRender();
		break;
	}
}
void Scene0116::Play()
{
	SpawnBlock();
	for (auto block : blocks)
		block->Update();

	leftBar->SetPress(KEY_PRESS(VK_LBUTTON));
	rightBar->SetPress(KEY_PRESS(VK_RBUTTON));
	
	leftBar->Update();
	rightBar->Update();
	ball->Update();

	if (ball->Pos().y < -ball->Half().y) {
		state = GameState::GAME_OVER;
		return;
	}
	Collide();
	if (state != GameState::PLAY)
		return;
}

void Scene0116::ClearRender()
{
	gameClearMessage->Render();
}

void Scene0116::GameOverRender()
{
	gameOverMessage->Render();
}

void Scene0116::CreateBar()
{
	float barLength = 130.0f;

	leftBar = new Bar0116({ barLength, 30.0f }, true);
	leftBar->SetTag("LeftBar");
	leftBar->Pos() = { CENTER_X - 90.0f, 100.0f };
	leftBar->Update();

	rightBar = new Bar0116({ barLength, 30.0f }, false);
	rightBar->SetTag("RightBar");
	rightBar->Pos() = { CENTER_X + 90.0f, 100.0f };
	rightBar->Update();
}

void Scene0116::CreateBlocks()
{
	int blockCol = 5;
	int blockRow = 3;
	float blockSizeX = (area->RightBottom().x - area->LeftTop().x) / (float)blockCol;
	float blockSizeY = 60.0f;

	blocks.resize(blockRow);
	for (auto& block : blocks) {
		block = new Block0116(Vector2(blockSizeX, blockSizeY));
		block->SetBreakFunc(bind(&Scene0116::BreakBlock, this));
	}
}

void Scene0116::StartGame()
{
	SetBlocks();
	SetBall();
	spawnTime = 0.0f;
}

void Scene0116::SetBlocks()
{
	int blockCol = 5;
	int blockRow = 3;
	float blockStartX = area->LeftTop().x;
	float blockStartY = area->LeftTop().y - 200.0f;
	float blockSizeX = (area->RightBottom().x - area->LeftTop().x) / (float)blockCol;
	float blockSizeY = 60.0f;

	score = 0;
	for(auto block : blocks)
		block->SetActive(false);
}

void Scene0116::SetBall()
{
	ball->Pos() = { CENTER_X + 30.0f, CENTER_Y };
	ball->GetVelocity() = { 0.0f, 0.0f };
}

void Scene0116::Collide()
{
	ball->BarCollision(leftBar);
	ball->BarCollision(rightBar);

	for (auto block : blocks) {
		//block 콜리전
		ball->BlockCollision(block);
	}

	for (auto block : blocks) {
		bool leftCol = leftBar->GetCollider()->IsCollision(block->GetCollider());
		bool rightCol = rightBar->GetCollider()->IsCollision(block->GetCollider());
		if (leftCol || rightCol) {
			state = GameState::GAME_OVER;
			return;
		}
	}

	ball->AreaCollision(area);
}

void Scene0116::BreakBlock()
{
	if (++score >= CLEAR_SCORE)
		state = GameState::CLEAR;
}

void Scene0116::SpawnBlock()
{
	//스폰 시간 검사
	spawnTime -= DELTA;
	if (spawnTime > 0.0f)
		return;

	Block0116* spawnBlock = nullptr;
	for (auto block : blocks) {
		if (!block->Active()) {
			spawnBlock = block;
			break;
		}
	}
	//스폰할 거 없음
	if (spawnBlock == nullptr)
		return;

	//스폰할 게 있음
	spawnTime = spawnRate;

	float left = area->LeftTop().x;
	float right = area->RightBottom().x;
	float top = area->LeftTop().y;

	Vector2 size = spawnBlock->GetCollider()->Size();

	float y = top - size.y * 0.5f;
	float minX = left + size.x * 0.5f;
	float maxX = right - size.x * 0.5f;

	spawnBlock->Pos() = { Random(minX, maxX), y };

	spawnBlock->Spawn();
}