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

cbuffer ProgressBuffer : register(b1)
{
    float ratio;
    float3 padding;
}

float4 GrayScale(float4 albedo)
{
    //회색으로 만들기
    //float scale = (albedo.r + albedo.g + albedo.b) / 3.0f;
    /*
    float3 grayColor =
    {
        albedo.r * 0.3f,
        albedo.g * 0.59f,
        albedo.b * 0.11f
    };
    float scale = grayColor.r + grayColor.g + grayColor.b;
    */
    //입력한 비율로 연산
    float scale = dot(albedo.rgb, float3(0.3f, 0.59f, 0.11f));
    
    return float4(scale.xxx, albedo.a);
}

float4 PS(PixelInput input) : SV_TARGET
{
    
    float4 tex = map.Sample(samp, input.uv);
    
    if (input.uv.x > ratio)
        return GrayScale(tex) * color;
    
    return tex;
}