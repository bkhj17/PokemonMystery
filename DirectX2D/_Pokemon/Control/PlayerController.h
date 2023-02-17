#pragma once
class Unit;
class PlayerController : public Controller
{
public:
    PlayerController();
    PlayerController(Unit* unit);
    ~PlayerController() = default;

    virtual bool SetCommand() override;
};

