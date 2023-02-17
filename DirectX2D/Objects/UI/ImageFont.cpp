#include "Framework.h"

ImageFont::ImageFont(wstring textureFile, float interval)
    : interval(interval)
{
    quads.resize(MAX_LENGTH);

    for(UINT i = 0 ; i < MAX_LENGTH ; i++)
    {
        quads[i] = new Quad(textureFile, Vector2(), Vector2(0.1f, 1.0f));                
        quads[i]->SetParent(this);
        quads[i]->SetPixelShader(L"ImageFont.hlsl");
    }

    fontSize = quads[0]->GetSize();

    valueBuffer = new IntValueBuffer();

    transit = new ColorBuffer;
    transit->Get() = { 1.0f, 0.0f, 1.0f, 1.0f };
}

ImageFont::~ImageFont()
{
    for (Quad* quad : quads)
        delete quad;

    delete valueBuffer;
    delete transit;
}

void ImageFont::Update()
{
    strValue = to_string(value);

    float start = 0.0f;
    float intervalSize = interval * (strValue.size() - 1);

    if (strValue.size() > 1)
    {
        switch (alignedType)
        {
        case ImageFont::LEFT:
            start = fontSize.x * strValue.size() + intervalSize;
            break;
        case ImageFont::CENTER:
            start = (fontSize.x * strValue.size() + intervalSize) * 0.5f;
            break;
        }
    }

    UpdateWorld();
    for(UINT i = 0 ; i < strValue.size(); i++)
    {
        quads[i]->Pos().x = i * -quads[i]->GetSize().x - interval * i + start;
        quads[i]->UpdateWorld();
    }

}

void ImageFont::Render()
{
    transit->SetPS(2);
    for (UINT i = 0; i < strValue.size(); i++)
    {
        int num = strValue[i] - '0';
        int index = strValue.size() - i - 1;

        valueBuffer->Get()[0] = num;
        valueBuffer->SetPS(1);

        quads[index]->Render();
    }
}

void ImageFont::PostRender()
{
    transit->SetPS(2);
    for (UINT i = 0; i < strValue.size(); i++)
    {
        int num = strValue[i] - '0';
        int index = strValue.size() - i - 1;

        valueBuffer->Get()[0] = num;
        valueBuffer->SetPS(1);

        quads[index]->PostRender();
    }
}

Vector2 ImageFont::GetSize()
{
    return Vector2(fontSize.x * strValue.size(), fontSize.y);
}
