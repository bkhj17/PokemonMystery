#pragma once

class DungeonTileMap : public GameTileMap
{
public:
    DungeonTileMap(string file);
    ~DungeonTileMap();

    virtual void Render() override;

    virtual void GetNodes(vector<Node*>& nodes) override;

    POINT PosToPoint(Vector2 pos);
    bool SetMove(IN int startX, IN int startY, IN int dirX, IN int dirY, OUT Vector2& destPos);
private:
    void SetGrid(int x, int y);

    void Load(string file) override;

    class DungeonAStar* astar;
};

