#pragma once

class AnimationScene :
    public Scene
{
public:
    AnimationScene();
    ~AnimationScene();

    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;
private:
    Ninja* ninja;

};

