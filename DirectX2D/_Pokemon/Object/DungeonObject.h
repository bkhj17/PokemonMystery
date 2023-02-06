#pragma once

class UnitMovement;
class DungeonObject :
    public GameObject
{
public:
    DungeonObject(Vector2 size);
    virtual ~DungeonObject();

    virtual void Update() = 0;
    virtual void UpdateWorld();
    virtual void Render();

    void SetPos(int x, int y);
    void SetMovePlan(int dirX, int dirY, int dist);

    UnitMovement* GetMovement() { return movement; }

    virtual bool IsActing();
    
protected:
    void MovementUpdate();

private:
    void SetMove();
protected:
    //콜라이더 없이 하는 게 낫다는 조언을 받았다
    RectCollider* collider;
    
    //뭔가에 부딪힐 때까지 이동하기
    int moveDist = 0;
    int moveDirX = 0, moveDirY = 0;
    UnitMovement* movement;
};

