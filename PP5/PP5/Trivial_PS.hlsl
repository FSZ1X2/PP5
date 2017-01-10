static float3 lightDir = float3(0.4, -0.3, -0.7);

struct OUTPUT_VERTEX
{
	float4 projectedCoordinate	: SV_POSITION;
	float3 normal				: NORMAL;
	float3 uv					: TEXCOORD0;
};

texture2D Texture : register(t0);
SamplerState tsampler:register(s0);

float4 main( OUTPUT_VERTEX input ) : SV_TARGET
{

	float3 dir = -normalize(lightDir);
	float3 normal = normalize(input.normal);
	float I = saturate(saturate(dot(dir, normal))+0.2) ;

	float4 color = Texture.Sample(tsampler, input.uv.xy);
	if (any(color))
	{
		color.xyz *= I;
		return color;
	}
	return (float4)I;
}