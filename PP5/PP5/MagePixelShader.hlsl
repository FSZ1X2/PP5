
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate	: SV_POSITION;
	float3 normal				: NORMAL;
	float3 uv					: TEXCOORD0;
	float4 WorldPos             : WORLDPOSITION;
	float3 tangent              : TANGENT;
	float3 binormal             : BINORMAL;
	float3 viewDir				: TEXCOORD1;
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

texture2D DiffTexture : register(t0);
texture2D NormTexture : register(t1);
texture2D SpecTexture : register(t2);

SamplerState tsampler:register(s0);

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	float3 viewdir = normalize(input.viewDir - input.WorldPos.xyz);
	float3 in_normal = normalize(input.normal);
	float3 tangent = normalize(input.tangent);
	float3 binormal = normalize(input.binormal);

	float3x3 TBN = float3x3(tangent, binormal, in_normal);

	float3 normal = NormTexture.Sample(tsampler, input.uv.xy).xyz;
	float3 spec = SpecTexture.Sample(tsampler, input.uv.xy).xyz;
	normal = (normal - 0.5) * 2;
	normal = normalize(mul(normal, TBN));

	float dotD = saturate(dot(normal, -normalize(DirectionalLight.xyz)));

	float3 lightDirP = (PointLightPosition.xyz - input.WorldPos.xyz);
	float distanceP = length(lightDirP);
	lightDirP /= distanceP;
	float dotP = saturate(dot(normal, lightDirP));
	float3 pcolor = PLcolor.xyz *dotP;
	float ATTENUATION = 1.0 - saturate((distanceP / lightradius.x));
	ATTENUATION *= ATTENUATION;
	pcolor = pcolor * ATTENUATION;

	float3 lightDirS = (SpotLightPosition.xyz - input.WorldPos.xyz);
	float distanceS = length(lightDirS);
	lightDirS /= distanceS;
	float dotS = saturate(dot(lightDirS, -normalize(conedir.xyz)));

	float spotfactor = dotS > coneratio.x;
	spotfactor *= saturate(dot(lightDirS, normalize(input.normal)));

	float3 dcolor = DLcolor.xyz *(dotD + 0.2f);
	float3 scolor = spotfactor*SLcolor.xyz;

	float3 H = normalize(viewdir + lightDirP);
	float S_intensity = pow(saturate( dot(H, normal) ), 22);
	float3 specular = S_intensity * spec / distanceP;

	H = normalize(viewdir + lightDirS);
	S_intensity = pow(saturate(dot(H, normal)), 22);
	specular += S_intensity * spec / distanceS;

	H = normalize(viewdir + DirectionalLight.xyz);
	S_intensity = pow(saturate(dot(H, normal)), 22);
	specular += S_intensity * spec;

	float3 combinecolor = saturate(dcolor + pcolor + scolor);

	float4 diffcolor = DiffTexture.Sample(tsampler, input.uv.xy);
	//float4 blendcolor = BlendTexture.Sample(tsampler, input.uv.xy);
	float4 color =diffcolor * float4(combinecolor, 1) + saturate(float4(specular,0));
	if (any(color))
		return color;
	return float4(combinecolor, 1);
}