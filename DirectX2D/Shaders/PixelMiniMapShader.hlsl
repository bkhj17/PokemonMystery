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
    int obj;
    //grid : LeftUp, Up, RightUp, Left, Right, LeftDown, Down, RightDown
    //obj : NONE, PLAYER, ENEMY, STAIR,	REFRESH, ITEM
}

float4 RenderWall(float2 uv)
{
    
    float l = 0.1f;
    float g = 1 - l;
    
    bool isWallRender = false;
    if (uv.y < l && !(grid & (1 << 1)))
        isWallRender = true;
    else if (uv.y > g && !(grid & (1 << 6)))
        isWallRender = true;
    else if (uv.x < l && !(grid & (1 << 3)))
        isWallRender = true;
    else if (uv.x > g && !(grid & (1 << 4)))
        isWallRender = true;
    
    float alpha = 0.2f;
    if (isWallRender)
        alpha = 1.0f;
    
    return float4(1.0f, 1.0f, 1.0f, alpha);
}

float4 RenderStair(float2 uv)
{
    float l = 0.1f;
    float g = 1 - l;
    
    if (uv.x < l || uv.y < l || uv.x > g || uv.y > g)
        return color;
    
    return RenderWall(uv);
}

float4 RenderCircle(float2 uv, float inCircle)
{
    float r2 = pow(0.5 - uv.x, 2) + pow(0.5 - uv.y, 2);
    if (r2 < pow(inCircle, 2))
        return color;
    return RenderWall(uv);
}

float4 PS(PixelInput input) : SV_TARGET
{
    float inRender = 0.5f;
    if(obj == 4)
        inRender = 0.1f;
    
    if (obj == 0)
        return RenderWall(input.uv);
    if (obj == 3)
        return RenderStair(input.uv);
    return RenderCircle(input.uv, inRender);
}