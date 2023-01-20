#pragma once

class Scene0120 : public Scene
{
private:
	enum State {
		READY, PLAY, CLEAR, GAME_OVER
	};
	State state = READY;

public:
	Scene0120();
	~Scene0120();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
private:
	void ScoreUp(int num);

private:

	class ProgressBar0120* timerBar;

	int goal;
	int score;

	Quad* clearMessage;
	Quad* gameOverMessage;
	const wstring CLEAR_MESSAGE = L"Textures/Shooting/GameClear.png";
	const wstring GAME_OVER_MESSAGE = L"Textures/Shooting/GameOver.png";
};

