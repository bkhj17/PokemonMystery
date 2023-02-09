#pragma once

class ProgressBar : public Quad
{
public:
    ProgressBar(wstring frontImageFile, wstring backImageFile);
    virtual ~ProgressBar();

    void Render();
    void PostRender();

    void SetAmount(float value);

protected:
    FloatValueBuffer* valueBuffer;
    Texture* backImage;

    float fillAmount = 1.0f;
};