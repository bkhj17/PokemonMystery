//PixelLight
struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Texture2D map : register(t0);
SamplerState samp : register(s0);

cbuffer IndexBuffer : register(b1)
{
    int index;
    int range;
    int squared;
    int lux;
}

cbuffer SizeBuffer : register(b2)
{
    float2 imageSize;
    float2 lightPos;
}

cbuffer PosBuffer : register(b3)
{
    float2 firstPos;
    float2 secondPos;
}

float4 Light(float2 uv)
{
    float4 albedo = map.Sample(samp, uv);
    
    float2 pixelCoord = uv * imageSize;
    
    float dist = distance(lightPos, pixelCoord);
    
    if(dist < range)
        return albedo;
    return float4(albedo.rgb * 0.1f, albedo.a);
}

float4 LightIntensity(float2 uv)
{
    float4 albedo = map.Sample(samp, uv);
    
    float2 pixelCoord = uv * imageSize;
    
    float dist = distance(lightPos, pixelCoord);
    
    float value = saturate(dist / range);
    float3 color = albedo.rgb - pow(value, squared);
    
    return float4(color, albedo.a);
}
float4 Light2(float2 uv)
{
    float4 albedo = map.Sample(samp, uv);
    float2 pixelCoord = uv * imageSize;
    float dist = distance(lightPos, pixelCoord);
        
    float value = dist / range;
    float3 color = albedo.rgb - pow(value, 2);
    
    return float4(albedo.rgb - value, albedo.a);
}

float4 DualLight(float2 uv)
{
    float4 albedo = map.Sample(samp, uv);
    
    float2 pixelCoord = uv * imageSize;
    
    float2 coord1 = firstPos;
    float2 coord2 = secondPos;
    float dist = min(distance(coord1, pixelCoord), distance(coord2, pixelCoord));
    
    if (dist < range)
        return albedo;
    return float4(albedo.rgb * 0.1f, albedo.a);
}


float4 PS(PixelInput input) : SV_TARGET
{
    if(index == 1)
        return LightIntensity(input.uv);        
    if(index == 2)
        return DualLight(input.uv);
    
    return Light(input.uv);
}