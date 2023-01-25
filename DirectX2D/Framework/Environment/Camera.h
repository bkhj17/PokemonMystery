#pragma once

class Quad;

class Camera : public Transform
{
public:
    Camera();
    ~Camera();

    void Update();

    void SetView();

    void SetTarget(Transform* target) { this->target = target; }

private:

    void FreeMode();
    void FollowMode();

private:
    MatrixBuffer* viewBuffer;
    Matrix view;

    float speed = 100.0f;

    Transform* target = nullptr;

};

