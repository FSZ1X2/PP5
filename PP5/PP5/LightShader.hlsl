
cbuffer ColorBuffer : register(b0)
{
	float4 color;
};

float4 main() : SV_TARGET
{
	return color;// float4(1.0f,0.0f,0.0f,1.0f);
}