struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Texture2D map : register(t0);
SamplerState samp : register(s0);

cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

cbuffer GridBuffer : register(b5)
{
    int grid;
    //LeftUp
    //Up
    //RightUp
    //Left
    //Right
    //LeftDown
    //Down
    //RightDown
}
float4 PS(PixelInput input) : SV_TARGET
{
    float l = 0.1f;
    float g = 1 - l;
    
    bool isRender = false;
    
    if(input.uv.y < l && !(grid & (1 << 1)))
        isRender = true;
    if(input.uv.y > g && !(grid & (1 << 6)))
        isRender = true;
    if (input.uv.x < l && !(grid & (1 << 3)))
        isRender = true;
    if (input.uv.x > g && !(grid & (1 << 4)))
        isRender = true;
    
    if(isRender)
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}