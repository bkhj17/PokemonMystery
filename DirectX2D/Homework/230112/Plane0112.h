#pragma once

class Plane0112 :
    public GameObject
{
private:
    const float SCALE_LENGTH = 100.0f;
    const float SCALE_WING = 100.0f;


    const float DEFAULT_SHOT_RATE = 0.5f;
    const float MIN_SHOT_RATE = 0.0f;
    const float MAX_SHOT_RATE = 1.0f;
    const float SHOT_RATE_ADD = 0.05f;

    const float COLLISION = SCALE_WING* 0.5f;
public:
    Plane0112();
    ~Plane0112();

    void Update();
    void Render();

    void Shot();
    void SetShotFunc(function<void(Vector2, float)> func) { shotFunc = func; }
public:
    class Transform0112* transform;

private:
    float moveSpeed = 100.0f;
    float rotateSpeed = 1.0f;

    float shotRate = DEFAULT_SHOT_RATE;
    float coolTime = 0.0f;

    Float3 shotPoint;

    function<void(Vector2, float)> shotFunc;
};

