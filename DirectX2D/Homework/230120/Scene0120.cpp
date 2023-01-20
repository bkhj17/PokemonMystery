#include "Framework.h"
#include "Scene0120.h"
#include "CardManager0120.h"
#include "ProgressBar0120.h"
#include "LineManager0120.h"

Scene0120::Scene0120()
{
	goal = 12;
	score = 0;
	CardManager0120::Get()->InitTable(goal);
	CardManager0120::Get()->SetMatchFunc(bind(&Scene0120::ScoreUp, this, 2));
	CardManager0120::Get()->Update();

	LineManager0120::Get();

	timerBar = new ProgressBar0120(L"Textures/Pinball/Bat.png");
	timerBar->Pos() = { CENTER_X, 100.0f };
	timerBar->Scale().x = 3.0f;
	timerBar->UpdateWorld();
	timerBar->Init(30.0f);

	clearMessage = new Quad(CLEAR_MESSAGE);
	clearMessage->Pos() = { CENTER_X, CENTER_Y };
	clearMessage->UpdateWorld();

	gameOverMessage = new Quad(GAME_OVER_MESSAGE);
	gameOverMessage->Pos() = { CENTER_X, CENTER_Y };
	gameOverMessage->UpdateWorld();
}

Scene0120::~Scene0120()
{
	CardManager0120::Delete();
	LineManager0120::Delete();

	delete timerBar;
	delete clearMessage;
	delete gameOverMessage;
}

void Scene0120::Update()
{
	switch (state)
	{
	case Scene0120::READY:
		if (KEY_DOWN(VK_LBUTTON))
			state = PLAY;
		break;
	case Scene0120::PLAY:
	{
		CardManager0120::Get()->SelectUpdate();

		timerBar->GetCurValue() -= DELTA;
		timerBar->UpdateWorld();
	}
		break;
	case Scene0120::CLEAR:
	case Scene0120::GAME_OVER:
		if (KEY_DOWN(VK_LBUTTON))
			state = READY;
		break;
	}

	CardManager0120::Get()->Update();
	LineManager0120::Get()->Update();
}

void Scene0120::Render()
{
	CardManager0120::Get()->Render();
	LineManager0120::Get()->Render();
	timerBar->Render();
}

void Scene0120::PostRender()
{

	switch (state)
	{
	case Scene0120::READY:
		Font::Get()->RenderText("Click to Start", { CENTER_X, CENTER_Y });
		break;
	case Scene0120::PLAY:
	break;
	case Scene0120::CLEAR:
		clearMessage->Render();
		break;
	case Scene0120::GAME_OVER:
		gameOverMessage->Render();
		break;
	}
}

void Scene0120::ScoreUp(int num)
{
	score += num;
	if (score >= goal) {
		state = CLEAR;
	}
}
