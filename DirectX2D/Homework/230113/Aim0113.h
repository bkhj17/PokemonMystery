#pragma once

class Aim0113 : public Quad
{
public:
    Aim0113();

    void Update();

    bool IsCollision(Quad* quad);
};

