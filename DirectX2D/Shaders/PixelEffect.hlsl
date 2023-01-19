struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Texture2D map : register(t0);
SamplerState samp : register(s0);

//
cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

//프레임 정보
cbuffer FrameBuffer : register(b1)
{
    float2 maxFrame; //프레임 범위
    float2 curFrame; //현재 프레임
}

float4 PS(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv + (curFrame / maxFrame);
    float4 tex = map.Sample(samp, uv);
    return tex * color;
}