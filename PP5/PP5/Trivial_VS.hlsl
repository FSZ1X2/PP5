struct INPUT_VERTEX
{
	float3 pos : POSITION;
	float3 color : UV;
	float3 normal : NORMAL;
	
};

struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float4 WorldPos : WORLDPOS;
	float4 CamWorldPos : CAMWORLDPOS;
};

cbuffer THIS_IS_VRAM : register( b0 )
{
	float4 constantColor;
	matrix World;
	matrix View;
	matrix Projection;
	float4 Camera;
};



OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output;
	
	output.colorOut = constantColor;
	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, World);
	output.WorldPos = pos;
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	output.pos = pos;
	output.normal = input.normal;
	output.CamWorldPos = Camera;
	return output;
}