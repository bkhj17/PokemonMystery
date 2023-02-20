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

    //condition에 해당하는 타일들 목록
    vector<POINT> GetPointsByCondition(function<bool(POINT)> condition);        
    //GetPointsByCondition로 받은 포인트 중 랜덤으로 1개 받기
    POINT GetRandomPointByCondition(function<bool(POINT)> condition);
    //플레이어 시작점 : 맵 로드할 때 랜덤 포인트 설정
    POINT GetPlayerStartPoint() { return playerStartPoint; }                
    //좌표에 해당하는 타일 반환
    DungeonBgTile* GetBgTile(POINT point);                                  
    
    //AStar용 노드
    struct DetectNode {
        pair<int, int> point = { -1, -1 };                                  //좌표
        int flag = 0;                                                       //더 탐색할지 판별하기 위한 
        int dist = 1000000000;                                              //시작점에서의 거리. 초기값은 크게 만든다
        
        //priority_queue에서 사용하기 위한 비교함수 정의. flag가 큰 지와 dist가 작은 지로 비교
        bool operator<(const DetectNode& node) const {
            return flag == node.flag ? dist < node.dist : flag > node.flag;
        }
    };

    //현 위치에서 인식할 수 있는 타일들
    vector<pair<int, int>> DetectableTiles(POINT curPoint);

private:
    //
    void SetGrid(int x, int y);

    void Load(string file) override;

    void SetUpTrap();
    void SetUpPlayerStart();
    void SetUpItems();
private:
    //인스턴스 렌더링을 위한 구조체
    struct TileInstanceData {
        Matrix transform;

        Vector2 maxFrame;
        Vector2 curFrame;

        int tileType;
        Float3 padding; //사이즈 맞추기 용 공간
    };

    vector<TileInstanceData> instances;
    VertexBuffer* instanceBuffer;
    Quad* quad;

    FloorData* floorData = nullptr;

    POINT playerStartPoint = {};
};

