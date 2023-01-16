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

    void SetBar();
    void CreateBlocks();
    void SetBlocks();

    void SetBall();

    void Collide();

    void BreakBlock();
private:
    Area0116* area;

    Bar0116* leftBar;
    Bar0116* rightBar;

    Ball0116* ball;

    int numBlock;
    vector<vector<Block0116*>> blocks;


    Quad* gameOverMessage = nullptr;
    Quad* gameClearMessage = nullptr;
};

