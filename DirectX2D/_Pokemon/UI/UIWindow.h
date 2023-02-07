#pragma once
class UIWindow :
    public Quad
{
public:
    UIWindow(Vector2 size, Vector2 pos);
    UIWindow(float left, float right, float up, float down);

    virtual void Init();
    virtual void Update() {}
    virtual void Render() {}
};
