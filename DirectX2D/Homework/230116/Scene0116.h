#pragma once

class Area0116;
class Bar0116;
class Ball0116;
class Block0116;

class Scene0116 : public Scene
{
private:
    enum class GameState {
        PLAY,
        CLEAR,
        GAME_OVER
    };
    GameState state = GameState::PLAY;

    const int CLEAR_SCORE = 10;

public:
    Scene0116();
    ~Scene0116();

    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;
private:
    void Play();
    void ClearRender();
    void GameOverRender();

    void CreateBar();
    void CreateBlocks();

    void StartGame();
    void SetBlocks();
    void SetBall();

    void Collide();

    void BreakBlock();

    void SpawnBlock();
private:
    Area0116* area;

    Bar0116* leftBar;
    Bar0116* rightBar;

    Ball0116* ball;

    int score = 0;
    vector<Block0116*> blocks;

    float spawnRate = 3.0f;
    float spawnTime = 0.0f;

    Quad* gameOverMessage = nullptr;
    Quad* gameClearMessage = nullptr;

};

