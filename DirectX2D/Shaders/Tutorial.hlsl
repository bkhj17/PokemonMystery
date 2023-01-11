//Sementic : 매개변수 및 반환값 정보를 어떻게 사용할지에 대한 의미
//SV : System Value

float4 VS( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

float4 PS() : SV_TARGET
{
    return float4(1.0f, 1.0f, 0.0f, 1.0f);
}