#pragma once
class UIWindow : public Quad
{
private:
    const Vector2 DEFAULT_CURSOR_SIZE = { 40.0f, 40.0f };
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
    void CreateCursor(Vector2 size);

protected:
    FloatValueBuffer* floatBuffer;

    Vector2 cursorSize;
    Quad* cQuad = nullptr;
    int cursor = 0;
    int maxCursor = 1;
};

