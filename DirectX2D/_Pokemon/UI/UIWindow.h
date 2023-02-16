#pragma once
class UIWindow : public Quad
{
public:
    UIWindow(Vector2 size, Vector2 pos);
    UIWindow(float left, float right, float up, float down);
    virtual ~UIWindow();

    virtual void Init();

    virtual void Update();

    virtual void RenderCursor();
    virtual void PostRender() override;

    virtual void Close();
private:
    void SetShaderInfo();
    void CreateCursor();

protected:
    FloatValueBuffer* floatBuffer;

    Quad* cQuad = nullptr;
    int cursor = 0;
    int maxCursor = 1;
};

