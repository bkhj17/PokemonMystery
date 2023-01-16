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
	SetBall();
	
	SetBar();

	CreateBlocks();
	SetBlocks();



	gameOverMessage = new Quad(L"Textures/Shooting/GameOver.png", { CENTER_X, CENTER_Y });
	gameClearMessage = new Quad(L"Textures/Shooting/GameClear.png", { CENTER_X, CENTER_Y });
}

Scene0116::~Scene0116()
{
	delete area;
	delete leftBar;
	delete rightBar;

	for (int i = 0; i < blocks.size(); i++) {
		for (auto block : blocks[i])
			delete block;
	}
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
		if (KEY_DOWN(VK_LBUTTON) || KEY_DOWN(VK_RBUTTON)) {
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

	for (int i = 0; i < blocks.size(); i++) {
		for (auto block : blocks[i])
			block->Render();
	}
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

	leftBar->SetPress(KEY_PRESS(VK_LBUTTON));
	rightBar->SetPress(KEY_PRESS(VK_RBUTTON));

	Collide();
	if (state != GameState::PLAY)
		return;

	ball->AreaCollision(area);

	for (int i = 0; i < blocks.size(); i++)
		for (auto block : blocks[i])
			block->Update();

	leftBar->Update();
	rightBar->Update();
	ball->Update();

	if (ball->Pos().y < 0.0f)
		state = GameState::GAME_OVER;
}

void Scene0116::ClearRender()
{
	gameClearMessage->Render();
}

void Scene0116::GameOverRender()
{
	gameOverMessage->Render();
}

void Scene0116::SetBar()
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
	float blockSizeY = 20.0f;

	blocks.resize(blockRow);
	for (int i = 0; i < blocks.size(); i++) {
		blocks[i].resize(blockCol);
		for (int j = 0; j < blocks[i].size(); j++) {
			blocks[i][j] = new Block0116(Vector2(blockSizeX, blockSizeY));
			blocks[i][j]->SetBreakFunc(bind(&Scene0116::BreakBlock, this));
		}
	}
}

void Scene0116::SetBlocks()
{
	int blockCol = 5;
	int blockRow = 3;
	float blockStartX = area->LeftTop().x;
	float blockStartY = area->LeftTop().y - 200.0f;
	float blockSizeX = (area->RightBottom().x - area->LeftTop().x) / (float)blockCol;
	float blockSizeY = 20.0f;

	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].size(); j++) {
			blocks[i][j]->SetActive(true);
			blocks[i][j]->Pos().x = blockStartX + ((float)j + 0.5f) * blockSizeX;
			blocks[i][j]->Pos().y = blockStartY + blockSizeY * (float)i;
			blocks[i][j]->UpdateWorld();
		}
	}

	numBlock = blockCol * blockRow;
}

void Scene0116::SetBall()
{
	ball->Pos() = { CENTER_X - 30.0f, CENTER_Y };
	ball->GetDirection() = { 0.0f, 0.0f };
}

void Scene0116::Collide()
{

	ball->BarCollision(leftBar);
	ball->BarCollision(rightBar);

	for (int i = 0; i < blocks.size(); i++) {
		for (auto block : blocks[i]) {
			//block ÄÝ¸®Àü
			ball->BlockCollision(block);
		}
	}

	for (int i = 0; i < blocks.size(); i++) {
		for (auto block : blocks[i]) {
			if (leftBar->GetCollider()->IsCollision(block->GetCollider())
				|| rightBar->GetCollider()->IsCollision(block->GetCollider())) {
				state = GameState::GAME_OVER;
				return;
			}
		}
	}
}

void Scene0116::BreakBlock()
{
	if (--numBlock <= 0)
		state = GameState::CLEAR;
}
