struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    
    float2 maxFrame : INSTANCE_MAX; //프레임 범위
    float2 curFrame : INSTANCE_CUR; //현재 프레임
};

Texture2D map : register(t0);
SamplerState samp : register(s0);

//
cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv + (input.curFrame / input.maxFrame);
    float4 tex = map.Sample(samp, uv);
    return tex * color;
}