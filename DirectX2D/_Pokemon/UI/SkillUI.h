#pragma once
#include "UIWindow.h"
class SkillUI : public UIWindow
{
private:
    const int CLOSE_CURSOR = 4;

public:
    SkillUI();
    ~SkillUI() = default;

    void Update();

private:
    int cursor = 0;
};

