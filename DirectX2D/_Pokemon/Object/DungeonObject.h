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
    //�ݶ��̴� ���� �ϴ� �� ���ٴ� ������ �޾Ҵ�
    RectCollider* collider;
    
    //������ �ε��� ������ �̵��ϱ�
    int moveDist = 0;
    int moveDirX = 0, moveDirY = 0;
    UnitMovement* movement;
};

