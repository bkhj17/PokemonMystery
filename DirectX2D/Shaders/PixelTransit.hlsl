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

cbuffer AlphaBuffer : register(b1)
{
    float4 transit;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 tex = map.Sample(samp, input.uv);
    if (tex.r == transit.r && tex.g == transit.g && tex.b == transit.b) 
        return float4(0, 0, 0, 0);
    return map.Sample(samp, input.uv) * color;
}