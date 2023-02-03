struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Texture2D frontMap : register(t0);
Texture2D backMap : register(t1);

SamplerState samp : register(s0);

cbuffer ValueBuffer : register(b1)
{
    float fillAmount;
}

float4 PS(PixelInput input) : SV_TARGET
{
    if (input.uv.x < fillAmount)
        return frontMap.Sample(samp, input.uv);
	
    return backMap.Sample(samp, input.uv);
}