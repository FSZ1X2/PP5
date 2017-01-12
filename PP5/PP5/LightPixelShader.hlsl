
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate	: SV_POSITION;
	float3 normal				: NORMAL;
	float3 uv					: TEXCOORD0;
	float4 WorldPos             : WORLDPOSITION;
};

cbuffer DirectionalLightConstantBuffer : register(b0)
{
	float4 DirectionalLight;
	float4 DLcolor;
};

cbuffer PointLightConstantBuffer : register(b1)
{
	float4 PointLightPosition;
	float4 PLcolor;
	float4 lightradius;
};

cbuffer SpotLightConstantBuffer : register(b2)
{
	float4 SpotLightPosition;
	float4 SLcolor;
	float4 conedir;
	float4 coneratio;

};

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	return float4(combinecolor, 1);
}