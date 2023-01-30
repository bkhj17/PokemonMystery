#pragma once

class Enemy0130;
class Scene0130 : public Scene
{
private:
	enum GameState {
		PLAY,
		CLEAR,
		GAME_OVER
	};
public:
	Scene0130();
	~Scene0130();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
private:
	void SetGameState(GameState state) { this->state = state; }

	void EnemyCallPath(void* caller);
	void GetPlayerPos(void* pos);
private:
	GameState state = PLAY;


	class TileMap0130* tileMap;
	class Pacman0130* pacman;

	Quad* gameOver = nullptr;
	Quad* gameClear = nullptr;

	AStar* astar;
	vector<Enemy0130*> enemies;

};

