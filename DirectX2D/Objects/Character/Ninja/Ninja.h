#pragma once

class Ninja : public GameObject
{
private:
    enum ActionType : int
    {
        IDLE = 0, RUN,
        JUMP_UP, JUMP_DOWN, GLIDE,
        ATTACK = 10,
        JUMP_ATTACK, THROW, JUMP_THROW,
        DEAD, SLIDE, 
    };

public:
    Ninja();
    ~Ninja();

    void Update();
    void Render();

private:
    void Control();

    void Move();
    void Jump();
    void Attack();
    void Glide();


    void SetIdle();
    void Fall();

    void Throw();

    void SetAction(ActionType type);
    void SetAction2(int type);

    void CreateActions();    

private:
    map<ActionType, Action*> actions;

    ActionType curAction = JUMP_DOWN;
};