static float3 lightDir = float3(-0.4, -0.4, 0.4);

struct OUTPUT_VERTEX
{
	float4 projectedCoordinate	: SV_POSITION;
	float3 normal				: NORMAL;
	float3 uv					: TEXCOORD0;
};

float4 main( OUTPUT_VERTEX input ) : SV_TARGET
{
	float3 dir = normalize(lightDir);
	float3 normal = normalize(input.normal);
	float4 I = saturate(dot(dir, normal));
	return I;
}