struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Texture2D map : register(t0);   //테두리 
SamplerState samp : register(s0);
cbuffer ColorBuffer : register(b0)
{
    float4 color;               //안쪽 색깔
}

cbuffer ValueBuffer : register(b1)
{
    float2 weight;              //두께 (0~1)
    float value;                //안쪽 투명도
}

float4 PS(PixelInput input) : SV_TARGET
{
    bool isFrame = false;
    float2 uv;
    
    float p1 = 1.0 / 3.0;
    float p2 = p1 * 2.0;
    
    if (input.uv.x < weight.x)
    {
        isFrame = true;
        uv.x = input.uv.x / weight.x * p1;
    }
    else if (input.uv.x > 1 - weight.x)
    {
        isFrame = true;
        uv.x = p2 + (input.uv.x - (1 - weight.x)) / weight.x * p1;
    }
    else
        uv.x = p1 + (input.uv.x - weight.x) / (1 - 2 * weight.x) * p1;
    
    if (input.uv.y < weight.y)
    {
        isFrame = true;
        uv.y = input.uv.y / weight.y * p1;
    }
    else if (input.uv.y > 1 - weight.y)
    {
        isFrame = true;
        uv.y = p2 + (input.uv.y - (1 - weight.y)) / weight.y * p1;
    }
    else
        uv.y = p1 + (input.uv.y - weight.y) / (1 - 2 * weight.y) * p1;
    
    if (isFrame)
        return map.Sample(samp, uv);
    
    
    return float4(color.rgb, value);
}