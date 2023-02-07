#pragma once
#include "Controller.h"

class Unit;
class PlayerController : public Controller
{
public:
    PlayerController() = default;
    PlayerController(Unit* unit);
    ~PlayerController() = default;

    virtual void SetCommand() override;
};

