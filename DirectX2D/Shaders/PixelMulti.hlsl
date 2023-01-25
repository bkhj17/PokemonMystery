//PixelMulti.hlsl
struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Texture2D map : register(t0);
Texture2D secondMap : register(t1);

SamplerState samp : register(s0);

cbuffer IndexBuffer : register(b1)
{
    int index;
}

float4 Modulate(float2 uv)
{
    float4 tex1 = map.Sample(samp, uv);
    float4 tex2 = secondMap.Sample(samp, uv);
    
    return tex1 * tex2;
}

float4 Modulate2(float2 uv)
{
    float4 tex1 = map.Sample(samp, uv);
    float4 tex2 = secondMap.Sample(samp, uv);
    
    return tex1 * tex2 * 2.0f;
}

float4 Add(float2 uv)
{
    float4 tex1 = map.Sample(samp, uv);
    float4 tex2 = secondMap.Sample(samp, uv);
    
    return tex1 + tex2;
}

float4 AddSmooth(float2 uv)
{
    float4 tex1 = map.Sample(samp, uv);
    float4 tex2 = secondMap.Sample(samp, uv);
    
    return tex1 + tex2 - tex1 * tex2;
}

float4 AddSigned(float2 uv)
{
    float4 tex1 = map.Sample(samp, uv);
    float4 tex2 = secondMap.Sample(samp, uv);
    
    return tex1 + tex2 - 0.5f;
}

float4 AddSigned2X(float2 uv)
{
    float4 tex1 = map.Sample(samp, uv);
    float4 tex2 = secondMap.Sample(samp, uv);
    
    return (tex1 + tex2 - 0.5f) * 2.0f;
}

float4 Subtract(float2 uv)
{
    float4 tex1 = map.Sample(samp, uv);
    float4 tex2 = secondMap.Sample(samp, uv);
    
    return float4((tex1 - tex2).rgb, max(tex1.a, tex2.a));
}

float4 RevSubtract(float2 uv)
{
    float4 tex1 = map.Sample(samp, uv);
    float4 tex2 = secondMap.Sample(samp, uv);
    
    return float4((tex2 - tex1).rgb, max(tex1.a, tex2.a));
}

float4 Max(float2 uv)
{
    float4 tex1 = map.Sample(samp, uv);
    float4 tex2 = secondMap.Sample(samp, uv);
    
    return float4((tex2 - tex1).rgb, max(tex1.a, tex2.a));
}

float4 PS(PixelInput input) : SV_TARGET
{
    if (index == 1)
        return Modulate(input.uv);
    if (index == 2)
        return Modulate2(input.uv);
    if (index == 3)
        return Add(input.uv);
    if (index == 4)
        return AddSmooth(input.uv);
    if (index == 5)
        return AddSigned(input.uv);
    if (index == 6)
        return AddSigned2X(input.uv);
    if (index == 7)
        return Subtract(input.uv);
    if (index == 8)
        return RevSubtract(input.uv);
    
    return map.Sample(samp, input.uv);
}