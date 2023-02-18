#pragma once

class ObjectMovement;
class DungeonObject :
    public GameObject
{
public:
    DungeonObject(Vector2 size);
    virtual ~DungeonObject();

    virtual void Update() = 0;
    virtual void UpdateWorld();
    virtual void Render() {}

    void SetPoint(int x, int y);
    void SetPoint(POINT point) { SetPoint(point.x, point.y); }

    POINT GetPoint();

    void SetMovePlan(int dirX, int dirY, int dist);

    virtual bool IsActing();
    bool IsMoving();
    
protected:
    void MovementUpdate();
private:
    void SetMove();

    virtual bool IsCollide() = 0;
    virtual void CollideFunc() { moveDist = 0; };
protected:    
    //¹º°¡¿¡ ºÎµúÈú ¶§±îÁö ÀÌµ¿ÇÏ±â
    int moveDist = 0;
    int moveDirX = 0, moveDirY = 0;
    ObjectMovement* movement;
};

