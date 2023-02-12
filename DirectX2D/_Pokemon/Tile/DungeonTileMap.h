#pragma once
struct FloorData;
class DungeonBgTile;
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

    DungeonBgTile* GetBgTile(POINT point);
    
    struct DetectNode {
        pair<int, int> point = { -1, -1 };
        int flag = 0;
        int dist = 1000000000; //시작점에서의 거리
        

        bool operator<(const DetectNode& node) const {
            return flag == node.flag ? dist < node.dist : flag > node.flag;
        }
    };

    struct ChaseNode {
        pair<int, int> point = { -1, -1 };
        pair<int, int> post = { -1, -1 };
        int dist = 1000000000;

        bool operator<(const ChaseNode& node) const {
            return dist < node.dist;
        }
    };

    //현 위치에서 인식할 수 있는 타일들
    vector<pair<int, int>> DetectableTiles(POINT curPoint);
    pair<int, int> ChasingPoint(const pair<int, int>& start, const pair<int, int>& target) const;

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

