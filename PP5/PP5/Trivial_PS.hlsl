//static float3 lightDir = float3(0.4, -0.3, -0.7);
//
//static float3 tdirection = float3(-5.0f, 2.0f, 0.0f);
//static float3 tDcolor    = float3(1.0f,1.0f,1.0f);
//			
//static float3 tPointpos    = float3( 0.0f,0.0f,-1.0f);
//static float3 tPcolor      = float3( 0.0f,0.0f,1.0f  );
//static float3 tlightradius = float3( 0.5f,0.0f,0.0f  );
//			
//static float3 tSpotpos    = float3( 0.0f,2.0f,0.0f );
//static float3 tScolor     = float3( 1.0f,0.0f,0.0f );
//static float3 tconedir    = float3( 0.0f,-1.0f,0.0f);
//static float3 tconeratio  = float3( 0.8f,0.0f,0.0f );

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

texture2D Texture : register(t0);
SamplerState tsampler:register(s0);

float4 main( OUTPUT_VERTEX input ) : SV_TARGET
{
	float3 lightDirP = normalize(PointLightPosition.xyz - input.WorldPos.xyz);
	float3 LightDirS = normalize(SpotLightPosition.xyz - input.WorldPos.xyz);

	float dotD = clamp(dot(input.normal, normalize(DirectionalLight.xyz)), 0, 1);
	float dotP = clamp(dot(input.normal, lightDirP), 0, 1);
	float dotS = clamp(dot(-LightDirS, normalize(conedir.xyz)), 0, 1);

	float spotfactor = (dotS > coneratio.x) ? 1 : 0;
	spotfactor *= clamp(dot(LightDirS, normalize(input.normal)), 0, 1);

	float3 dcolor = DLcolor.xyz *clamp((dotD + 0.2f),0,1);
	float3 pcolor = PLcolor.xyz *dotP;
	float3 scolor = spotfactor*SLcolor.xyz;

	float ATTENUATION = 1.0 - clamp((lightDirP / lightradius.x), 0, 1);

	float3 pcolor2 = pcolor * ATTENUATION;
	float3 combinecolor = clamp(dcolor + pcolor2 + scolor, 0, 1);

	//float3 dir = -normalize(tdirection);
	//float3 normal = normalize(input.normal);
	//float I = saturate(saturate(dot(dir, normal))+0.2);
	//float3 light = clamp(I* tDcolor, 0, 1);

	//float3 lightDirP = normalize(tPointpos - input.WorldPos.xyz);
	//float3 LightDirS = normalize(tSpotpos - input.WorldPos.xyz);

	//float dotD = clamp(dot(normalize(input.normal), -normalize(tdirection)), 0, 1);
	//float dotP = clamp(dot(input.normal, lightDirP), 0, 1);
	//float dotS = clamp(dot(-LightDirS, normalize(tconedir.xyz)), 0, 1);

	//float spotfactor = (dotS > tconeratio.x) ? 1 : 0;
	//spotfactor *= clamp(dot(LightDirS, normalize(input.normal)), 0, 1);

	//float3 dcolor = tDLcolor*clamp((dotD + 0.2f),0,1);
	//float3 pcolor = tPLcolor.xyz *dotP;
	//float3 scolor = spotfactor*tScolor.xyz;

	//float ATTENUATION = 1.0 - clamp((lightDirP / tlightradius.x), 0, 1);

	//float3 pcolor2 = pcolor * ATTENUATION;
	//float3 combinecolor = clamp(dcolor/* + pcolor2 + scolor*/, 0, 1);
	float4 surfaceColor = float4(0.15,0.15,0.15,0.15);
	float4 color = Texture.Sample(tsampler, input.uv.xy) * float4(combinecolor,1);
	if(input.uv.x != -1)
	return color;

	return surfaceColor *float4(combinecolor, 1);
	/*if (any(color))
	{
		color.xyz *= I;
		return color;
	}
	return (float4)I;*/
}