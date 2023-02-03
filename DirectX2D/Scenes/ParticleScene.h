#pragma once

class ParticleScene : public Scene
{
private:
    const UINT MAX_PARTICLE = 500;

    struct InstanceData
    {
        Matrix transform;
        Vector2 maxFrame = { 1, 1 };
        Vector2 curFrame = { 1, 1 };
        UINT textureIndex = 0;
    };

    struct ParticleData
    {
        UINT count = 100;
        float duration = 1.0f;
        Vector2 minVelocity = { -1, -1 };
        Vector2 maxVelocity = { +1, +1 };
        Vector2 minAccelation = { 0, 0 };
        Vector2 maxAccelation = { 0, 0 };
        float minSpeed = 1.0f;
        float maxSpeed = 100.0f;
        float minAngularVelocity = -10.0f;
        float maxAngularVelocity = +10.0f;
        float minStartTime = 0.0f;
        float maxStartTime = 0.0f;
        Vector2 minScale = { 1, 1 };
        Vector2 maxScale = { 1, 1 };
        Float4 startColor = { 1, 1, 1, 1 };
        Float4 endColor = { 1, 1, 1, 1 };
    };

    struct ParticleInfo
    {
        Vector2 velocity;
        Vector2 accelation;
        Vector2 scale;
        float speed;
        float angularVelocity;
        float startTime;
        UINT textureIndex;
    };

public:
    ParticleScene();
    ~ParticleScene();

    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;

    void Play();

private:
    void UpdateParticleInfo();
    void Start();

    void EditTexture();
    void SaveData(string file);
    void LoadData(string file);

    void SaveDialog();
    void LoadDialog();

private:
    Quad* quad;
    vector<InstanceData> instances;
    vector<ParticleInfo> particleInfos;
    vector<Transform> transforms;

    VertexBuffer* instanceBuffer;

    ParticleData data;

    float lifeTime = 0.0f;
    Float4 color;
    UINT particleCount = MAX_PARTICLE;

    Vector2 startPos = { CENTER_X, CENTER_Y };

    string projectPath;

    vector<ID3D11ShaderResourceView*> textureArray;
    vector<Texture*> textures;
};