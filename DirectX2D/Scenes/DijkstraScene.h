#pragma once

class DijkstraScene : public Scene
{
public:
    DijkstraScene();
    ~DijkstraScene();

    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;

private:
    Dijkstra* dijkstra;

    Tank* tank;
};

