#pragma once
class PuzzleScene : public Scene
{
public:
    PuzzleScene();
    ~PuzzleScene();

    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;

private:
    void LoadTextures();

private:
    vector<wstring> cardTextures;

    Button* button;
};

