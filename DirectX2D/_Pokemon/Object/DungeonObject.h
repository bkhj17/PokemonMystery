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

    void SetPoint(int x, int y);
    void SetPoint(POINT point) { SetPoint(point.x, point.y); }
    void SetMovePlan(int dirX, int dirY, int dist);

    virtual bool IsActing();
    
protected:
    void MovementUpdate();

private:
    void SetMove();

    virtual bool IsCollide() = 0;
    virtual void CollideFunc() { moveDist = 0; };
protected:
    //�ݶ��̴� ���� �ϴ� �� ���ٴ� ������ �޾Ҵ�
    RectCollider* collider;
    
    //������ �ε��� ������ �̵��ϱ�
    int moveDist = 0;
    int moveDirX = 0, moveDirY = 0;
    UnitMovement* movement;
};

