#pragma once
struct FloorData;
class DungeonBgTile;
class DungeonTileMap : public GameTileMap
{
private:
    UINT MAX_WIDTH = 200;
    UINT MAX_HEIGHT = 200;
public:
    DungeonTileMap();
    ~DungeonTileMap();
    
    void Init(string key, int floorNum);

    virtual void UpdateWorld() override;
    virtual void Render() override;

    POINT PosToPoint(Vector2 pos);
    bool SetMove(IN int startX, IN int startY, IN int dirX, IN int dirY, OUT Vector2& destPos);

    //
    FloorData* GetFloorData() { return floorData; }

    //condition�� �ش��ϴ� Ÿ�ϵ� ���
    vector<POINT> GetPointsByCondition(function<bool(POINT)> condition);        
    //GetPointsByCondition�� ���� ����Ʈ �� �������� 1�� �ޱ�
    POINT GetRandomPointByCondition(function<bool(POINT)> condition);
    //�÷��̾� ������ : �� �ε��� �� ���� ����Ʈ ����
    POINT GetPlayerStartPoint() { return playerStartPoint; }                
    //��ǥ�� �ش��ϴ� Ÿ�� ��ȯ
    DungeonBgTile* GetBgTile(POINT point);                                  
    
    //AStar�� ���
    struct DetectNode {
        pair<int, int> point = { -1, -1 };                                  //��ǥ
        int flag = 0;                                                       //�� Ž������ �Ǻ��ϱ� ���� 
        int dist = 1000000000;                                              //������������ �Ÿ�. �ʱⰪ�� ũ�� �����
        
        //priority_queue���� ����ϱ� ���� ���Լ� ����. flag�� ū ���� dist�� ���� ���� ��
        bool operator<(const DetectNode& node) const {
            return flag == node.flag ? dist < node.dist : flag > node.flag;
        }
    };

    //�� ��ġ���� �ν��� �� �ִ� Ÿ�ϵ�
    vector<pair<int, int>> DetectableTiles(POINT curPoint);

private:
    //
    void SetGrid(int x, int y);

    void Load(string file) override;

    void SetUpTrap();
    void SetUpPlayerStart();
    void SetUpItems();
private:
    //�ν��Ͻ� �������� ���� ����ü
    struct TileInstanceData {
        Matrix transform;

        Vector2 maxFrame;
        Vector2 curFrame;

        int tileType;
        Float3 padding; //������ ���߱� �� ����
    };

    vector<TileInstanceData> instances;
    VertexBuffer* instanceBuffer;
    Quad* quad;

    FloorData* floorData = nullptr;

    POINT playerStartPoint = {};
};

