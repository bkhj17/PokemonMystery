#pragma once
class RenderTargetScene : public Scene
{
public:
    RenderTargetScene();
    ~RenderTargetScene();

    // Scene을(를) 통해 상속됨
    virtual void Update() override;

    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
private:
    Quad* bg;
    Plane* plane;
    Ninja* ninja;

    RenderTarget* renderTarget;
    RenderTarget* ninjaRenderTarget;
    vector<Quad*> renderTextures;

    IntValueBuffer* intValueBuffer;
    FloatValueBuffer* floatValueBuffer;
    FloatValueBuffer* lightBuffer;

    Camera* planeCamera;
    Camera* ninjaCamera;
};

