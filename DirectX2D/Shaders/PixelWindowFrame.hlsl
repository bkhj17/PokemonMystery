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
    float2 weight;              //x 두께 (0~1)
    float value;                //안쪽 투명도
}

float4 PS(PixelInput input) : SV_TARGET
{
    bool isFrame = false;
    float2 uv;
    
    if (input.uv.x < weight.x)
    {
        isFrame = true;
        uv.x = input.uv.x / weight.x * 0.33;
    }
    else if (input.uv.x > 1 - weight.x)
    {
        isFrame = true;
        uv.x = 0.66 + (input.uv.x - (1 - weight.x)) / weight.x * 0.33;
    }
    else
    {
        uv.x = 0.33 + (input.uv.x - weight.x) / (1 - 2 * weight.x) * 0.33;
    }
    
    if (input.uv.y < weight.y)
    {
        isFrame = true;
        uv.y = input.uv.y / weight.y * 0.33;
    }
    else if (input.uv.y > 1 - weight.y)
    {
        isFrame = true;
        uv.y = 0.66 + (input.uv.y - (1 - weight.y)) / weight.y * 0.33;
    }
    else
    {
        uv.y = 0.33 + (input.uv.y - weight.y) / (1 - 2 * weight.y) * 0.33;
    }
    
    if (isFrame)
    {
        return map.Sample(samp, uv);  
    }
    
    return float4(color.rgb, value);
}