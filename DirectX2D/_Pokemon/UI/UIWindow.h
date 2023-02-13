#pragma once
class UIWindow :
    public Quad
{
public:
    UIWindow(Vector2 size, Vector2 pos);
    UIWindow(float left, float right, float up, float down);
    virtual ~UIWindow();

    virtual void Init();

    virtual void Update();

    virtual void RenderCursor() = 0;
    virtual void PostRender() override;
private:
    void SetShaderInfo();
private:
    FloatValueBuffer* floatBuffer;
};

