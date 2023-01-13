#pragma once

class Aim0113;
class Clay0113;

class Scene0113 : public Scene
{
	enum class GameState {
		READY,
		PLAY,
		CLEAR,
		GAME_OVER
	};
	GameState state = GameState::READY;
public:
	Scene0113();
	~Scene0113();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void SpawnClay();

	float CalcClayY(float x);

	void Play();
	void ClearRender();
	void GameOverRender();

	void Reset();

	void Hit(); 
	void Out() { if (--life <= 0) state = GameState::GAME_OVER; }

	void CreateBackground();

private:
	void DrawScore();

private:
	const int DEFAULT_LIFE = 4;
	const int DEFAULT_GOAL = 10;

	Aim0113* leftAim;
	Aim0113* rightAim;

	vector<Clay0113*> clays;

	float spawnRate = 0.6f;
	float spawnTime = 0.0f;

	int score = 0;
	int goal = DEFAULT_GOAL;
	int life = DEFAULT_LIFE;

	Quad* gameOverMessage = nullptr;
	Quad* gameClearMessage = nullptr;


	Quad* background = nullptr;
};

