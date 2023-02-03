#pragma once

class AnimObject :
    public GameObject
{
public:
    AnimObject();
    ~AnimObject();

    virtual void Update();
    virtual void Render();

    void AddClip(int key, Clip* clip) { clips[key] = clip; }
    void SetClip(int key);
private:
    ColorBuffer* colorBuffer;

    map<int, Clip*> clips;
    Clip* curClip = nullptr;
};

