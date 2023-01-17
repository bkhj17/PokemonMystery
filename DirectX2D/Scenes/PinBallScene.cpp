#include "Framework.h"
#include "PinBallScene.h"

PinBallScene::PinBallScene()
{
	background = new Quad(L"Textures/Pinball/Base.png");
	background->Scale().x *= WIN_WIDTH / background->GetSize().x;
	background->Scale().y *= WIN_HEIGHT / background->GetSize().y;
	background->Pos() = { CENTER_X, CENTER_Y };
	background->UpdateWorld();

	leftBat = new Bat(false);
	leftBat->SetTag("leftBat");

	rightBat = new Bat(true);
	rightBat->SetTag("rightBat");

	ball = new Ball;
	ball->SetTag("Ball");
}

PinBallScene::~PinBallScene()
{
	delete background;
	delete leftBat;
	delete rightBat;
	delete ball;
}

void PinBallScene::Update()
{
	leftBat->Update();
	rightBat->Update();
	ball->Update();

	Collision();
}

void PinBallScene::Render()
{
	background->Render();
	leftBat->Render();
	rightBat->Render();
	ball->Render();
}

void PinBallScene::PostRender()
{
	leftBat->RenderUI();
	rightBat->RenderUI();
	ball->RenderUI();
}

void PinBallScene::Collision()
{
	if (leftBat->GetCollider()->IsCollision(ball->GetCollider())) {
		ball->CollisionBat(leftBat);
	}

	if (rightBat->GetCollider()->IsCollision(ball->GetCollider())) {
		ball->CollisionBat(rightBat);
	}
}
