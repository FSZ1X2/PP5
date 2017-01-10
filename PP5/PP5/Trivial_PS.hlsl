struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float4 WorldPos : WORLDPOS;
	float4 CamWorldPos : CAMWORLDPOS;
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

texture2D diffTexture : register(t0);

texture2D normTexture : register(t1);

texture2D specTexture : register(t2);

SamplerState filters[3] : register(s0);

float4 main( float4 colorFromRasterizer : COLOR, PixelShaderInput input) : SV_TARGET
{
	float SPECULARINTENSITY = 1.0f;
float SPECULARPOWER = 60.0f;

float3 lightDirP = normalize(PointLightPosition.xyz - input.WorldPos.xyz);
float3 LightDirS = normalize(SpotLightPosition.xyz - input.WorldPos.xyz);

float dotD = clamp(dot(input.normal, normalize(DirectionalLight.xyz)), 0, 1);
float dotP = clamp(dot(input.normal, lightDirP), 0, 1);
float dotS = clamp(dot(-LightDirS, normalize(conedir.xyz)), 0, 1);

float spotfactor = (dotS > coneratio.x) ? 1 : 0;
spotfactor *= clamp(dot(LightDirS, normalize(input.normal)), 0, 1);

float3 viewdir = normalize(input.CamWorldPos.xyz - input.WorldPos.xyz);
float3 revectorD = reflect(normalize(DirectionalLight.xyz), normalize(input.normal));
float3 revectorP = reflect(-lightDirP, normalize(input.normal));
float3 revectorS = reflect(-LightDirS, normalize(input.normal));

float intensityD = pow(dot(normalize(revectorD), viewdir), SPECULARPOWER);
float intensityP = pow(dot(normalize(revectorP), viewdir), SPECULARPOWER);
float intensityS = pow(dot(normalize(revectorS), viewdir), SPECULARPOWER);

float3 dcolor = DLcolor.xyz *clamp((dotD + 0.2f), 0, 1);
float3 pcolor = PLcolor.xyz *dotP;
float3 scolor = spotfactor*SLcolor.xyz;

float ATTENUATION = 1.0 - clamp((lightDirP / lightradius.x), 0, 1);

float3 pcolor2 = pcolor * ATTENUATION;

float3 white = float3(1.0, 1.0, 1.0);
float3 resultD = white * SPECULARINTENSITY * intensityD;
float3 resultP = white * SPECULARINTENSITY * intensityP;
float3 resultS = white * SPECULARINTENSITY * intensityS;

float3 combinecolor = clamp(dcolor + pcolor2 + scolor + resultD + resultP + resultS, 0, 1);

float3 baseColor = colorFromRasterizer*combinecolor;
//float a = (diffTexture.Sample(filters[0], input.color.xy)).a;
//if (a < 0.5)
//	discard;

return float4(baseColor, colorFromRasterizer.w);
	//return colorFromRasterizer;
}