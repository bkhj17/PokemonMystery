#pragma once
struct FloorData;
class DungeonTileMap : public GameTileMap
{
private:
    UINT MAX_WIDTH = 200;
    UINT MAX_HEIGHT = 200;
public:
    struct TileInstanceData {
        Matrix transform;

        Vector2 maxFrame;
        Vector2 curFrame;

        int tileType;
        Float3 padding;
    };

    DungeonTileMap();
    ~DungeonTileMap();

    void Init(string key, int floorNum);

    virtual void UpdateWorld() override;
    virtual void Render() override;

    virtual void GetNodes(vector<Node*>& nodes) override;

    POINT PosToPoint(Vector2 pos);
    bool SetMove(IN int startX, IN int startY, IN int dirX, IN int dirY, OUT Vector2& destPos);

    FloorData* GetFloorData() { return floorData; }

    vector<POINT> GetPointsByCondition(function<bool(POINT)> condition);
    POINT GetRandomPointByCondition(function<bool(POINT)> condition);

    POINT GetPlayerStartPoint() { return playerStartPoint; }

private:
    void SetGrid(int x, int y);
    void Load(string file) override;

    void SetUpTrap();
    void SetUpPlayerStart();
private:
    class DungeonAStar* astar;

    vector<TileInstanceData> instances;
    VertexBuffer* instanceBuffer;

    Quad* quad;

    FloorData* floorData = nullptr;

    POINT playerStartPoint = {};

};

