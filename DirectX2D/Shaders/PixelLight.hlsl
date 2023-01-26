//PixelLight
struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};
/*
struct LightPos
{
    int active;
    float2 lightPos;
    float padding;
};
*/
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
    float2 lightPos1;
    float2 lightPos2;
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


float LightIntensity(float2 uv, float2 lightPos)
{         
    uv.y = 1.0 - uv.y;
    
    float2 pixelCoord = uv * imageSize;
    
    float dist = distance(lightPos, pixelCoord);
    
    return saturate(dist / range);
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

float4 Light(float2 uv, float2 lightPos)
{
    float4 albedo = map.Sample(samp, uv);
    
    float2 pixelCoord = uv * imageSize;
    
    float dist = distance(lightPos, pixelCoord);
    
    float value = saturate(dist / range);
    float3 color = albedo.rgb - pow(value, squared);
    
    return float4(color, albedo.a);
}

float4 DualLight(float2 uv)
{
    float4 albedo = map.Sample(samp, uv);
    
    float2 pixelCoord = uv * imageSize;
    
    float dist = min(distance(lightPos1, pixelCoord), distance(lightPos2, pixelCoord));
    
    if (dist < range)
        return albedo;
    return float4(albedo.rgb * 0.1f, albedo.a);
}

float4 PS(PixelInput input) : SV_TARGET
{
    float dist1 = LightIntensity(input.uv, lightPos1);
    float dist2 = LightIntensity(input.uv, lightPos2);
    float dist = saturate(dist1 * dist2);
    float4 albedo = map.Sample(samp, input.uv);
    return float4(albedo.rgb - dist, albedo.a);
}