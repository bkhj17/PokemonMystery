#pragma once

class Clay0113 : public Quad
{
private:
    const float RADIUS = 30.0f;
public:
    Clay0113();
    ~Clay0113();

    void Update();
    void Spawn();

    void Hit();
    void Out();

    void SetHitFunc(function<void()> func) { hitFunc = func; }
    void SetOutFunc(function<void()> func) { outFunc = func; }

    const Vector2& GetDirection() { return velocity; }

    void ShutDown() { isActive = false; }
private:
    float time = 0.0f;
    float xSpeed = 0.0f;
    float yAccel = 0.0f;
    
    Vector2 velocity = {};

    function<void()> hitFunc = nullptr;
    function<void()> outFunc = nullptr;
};

