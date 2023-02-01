#pragma once
class UIScene : public Scene
{
public:
    UIScene();
    ~UIScene();

    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;

private:
    GameTileMap* tileMap;
    Isaac* isaac;

};

