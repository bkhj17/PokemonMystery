struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    
    float2 maxFrame : INSTANCE_MAX; //프레임 범위
    float2 curFrame : INSTANCE_CUR; //현재 프레임
    
    int tileType : INSTANCE_TYPE;
};

Texture2D wallMap : register(t0);
Texture2D landMap : register(t1);
Texture2D waterMap : register(t2);
SamplerState samp : register(s0);

cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv + (input.curFrame / input.maxFrame);
    
    if (input.tileType == 3)
        return landMap.Sample(samp, uv) * color;
    else if(input.tileType == 2)
        return waterMap.Sample(samp, uv) * color;
    else
        return wallMap.Sample(samp, uv) * color;
}