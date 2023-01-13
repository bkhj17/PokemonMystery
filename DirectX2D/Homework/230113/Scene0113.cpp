#include "Framework.h"
#include "Scene0113.h"
#include "Aim0113.h"
#include "Clay0113.h"

Scene0113::Scene0113()
{
	float fal = 150.0f;

	leftAim = new Aim0113;
	leftAim->Pos() = { CENTER_X - fal, CENTER_Y };
	rightAim = new Aim0113;
	rightAim->Pos() = { CENTER_X + fal, CENTER_Y };

	leftAim->Update();
	rightAim->Update();

	clays.resize(4);
	for (auto& clay : clays) {
		clay = new Clay0113();
		clay->SetHitFunc(bind(&Scene0113::Hit, this));
		clay->SetOutFunc(bind(&Scene0113::Out, this));
	}
	gameOverMessage = new Quad(L"Textures/Shooting/GameOver.png", { CENTER_X, CENTER_Y });
	gameClearMessage = new Quad(L"Textures/Shooting/GameClear.png", { CENTER_X, CENTER_Y });

	CreateBackground();

	Audio::Get()->Add("Hit", "Sounds/Explosion.wav");
}

Scene0113::~Scene0113()
{
	delete leftAim;
	delete rightAim;

	for (auto clay : clays)
		delete clay;

	delete gameClearMessage;
	delete gameOverMessage;
}

void Scene0113::Update()
{
	switch (state)
	{
	case Scene0113::GameState::READY:
		if (KEY_DOWN(VK_SPACE))
			state = GameState::PLAY;
		break;
	case Scene0113::GameState::PLAY:
		Play();
		break;
	case Scene0113::GameState::CLEAR:
	case Scene0113::GameState::GAME_OVER:
		if (KEY_DOWN(VK_SPACE))
			Reset();
	}
}

void Scene0113::Render()
{
	if (background != nullptr)
		background->Render();

	for(auto clay : clays)
		clay->Render();

	leftAim->Render();
	rightAim->Render();
}

void Scene0113::PostRender()
{
	DrawScore();

	switch (state)
	{
	case Scene0113::GameState::PLAY:
		break;
	case Scene0113::GameState::CLEAR:
		ClearRender();
		break;
	case Scene0113::GameState::GAME_OVER:
		GameOverRender();
		break;
	}
}

void Scene0113::SpawnClay()
{
	spawnTime -= DELTA;
	if (spawnTime > 0.0f)
		return;

	for (auto clay : clays) {
		if (!clay->IsActive()) {
			clay->Spawn();
			spawnTime = spawnRate;
			return;
		}
	}
}

float Scene0113::CalcClayY(float x)
{
	float targetDist = 1000000.0f;
	Clay0113* target = nullptr;

	for (auto c : clays) {
		if (!c->IsActive())
			continue;

		auto dist = x - c->Pos().x;

		if (c->GetDirection().x * dist < 0.0f) {
			//진행방향에 x가 없다면
			continue;
		}

		if (targetDist > abs(dist)) {
			targetDist = abs(dist);
			target = c;
		}
	}


	if (target != nullptr) {
		return target->Pos().y;
	}
	return -1.0f;
}

void Scene0113::Play()
{
	SpawnClay();

	for (auto clay : clays)
		clay->Update();

	float height = CalcClayY(leftAim->Pos().x);
	if (height > 0.0f)
		leftAim->Pos().y = height;

	height = CalcClayY(rightAim->Pos().x);
	if (height > 0.0f)
		rightAim->Pos().y = height;

	leftAim->Update();
	rightAim->Update();

	if (KEY_DOWN(VK_SPACE)) {
		for (auto clay : clays) {
			if (leftAim->IsCollision(clay) || rightAim->IsCollision(clay))
				clay->Hit();
		}
	}
}

void Scene0113::ClearRender()
{
	if (gameClearMessage != nullptr)
		gameClearMessage->Render();
}

void Scene0113::GameOverRender()
{
	if (gameOverMessage != nullptr)
		gameOverMessage->Render();
}

void Scene0113::Reset()
{
	leftAim->Pos().y = rightAim->Pos().y = CENTER_Y;
	leftAim->UpdateWorld();
	rightAim->UpdateWorld();

	for (auto clay : clays) {
		clay->ShutDown();
	}

	life = DEFAULT_LIFE;
	score = 0;

	state = GameState::READY;
}

void Scene0113::Hit() 
{
	Audio::Get()->Play("Hit");
	if (++score >= goal) 
		state = GameState::CLEAR; 
}

void Scene0113::CreateBackground()
{
	background = new Quad(L"Textures/Shooting/background.png");
	background->Scale().x *= 2.5f;
	background->Scale().y *= 2.5f;
	background->Pos().x = CENTER_X;
	background->Pos().y = CENTER_Y;
	background->UpdateWorld();
}

void Scene0113::DrawScore()
{
	ImGui::SetWindowSize({ 200.0f, 150.0f }, 0);

	string str;
	if (state == GameState::READY) {
		str = "Press Space to Start";
		ImGui::Text(str.c_str());
	}
	else {
		str = "Score : " + to_string(score);
		ImGui::Text(str.c_str());
		str = "Life : " + to_string(life);
		ImGui::Text(str.c_str());
	}
}

