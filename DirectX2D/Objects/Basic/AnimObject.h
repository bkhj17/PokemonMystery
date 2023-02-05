#pragma once

class AnimObject :
    public GameObject
{
    enum Direction {
        LEFT_UP, UP, RIGHT_UP,
        LEFT, RIGHT,
        LEFT_DOWN, DOWN, RIGHT_DOWN
    };


public:
    AnimObject();
    ~AnimObject();

    virtual void Update();
    virtual void Render();

    void AddClip(int key, Clip* clip) { clips[key] = clip; }
    void SetDir(int x, int y);
    void SetClip(int key);
private:
    ColorBuffer* colorBuffer;

    Direction direction = DOWN;

    map<int, Clip*> clips;
    Clip* curClip = nullptr;
};

