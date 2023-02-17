#pragma once
class SkillUI : public UIWindow
{
private:
    const int CLOSE_CURSOR = 4;

public:
    SkillUI();
    ~SkillUI() = default;

    void Update();
    void PostRender();

private:
    //위치 조정용 함수들
    float CursorX();
    float NameX();
    float PpX();

    float LineY(int lineNum);
};

