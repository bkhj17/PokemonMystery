struct VertexInput
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VertexOutput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.pos = input.pos;
    output.color = input.color;
    return output;
}