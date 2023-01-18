#pragma once
#include "NinjaAttack.h"
class NinjaJumpAttack : public NinjaAttack
{
public:
    NinjaJumpAttack(Transform* target);
    ~NinjaJumpAttack() = default;

    virtual void Update() override;

    virtual void Start() override;
private:
    virtual void DisableCollider() override;

private:
    float velocity = 0.0f;

    bool isFall = false;
};

