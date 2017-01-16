//struct INPUT_VERTEX
//{
//	float4 coordinate : POSITION;
//};
//
//struct OUTPUT_VERTEX
//{
//	float4 colorOut : COLOR;
//	float4 projectedCoordinate : SV_POSITION;
//};
//
//// TODO: PART 3 STEP 2a
//cbuffer THIS_IS_VRAM : register( b0 )
//{
//	float4 constantColor;
//	matrix World;
//	matrix View;
//	matrix Projection;
//};
//
////cbuffer ConstantBuffer : register(b0)
////{
////	
////}
//
//OUTPUT_VERTEX main(INPUT_VERTEX input)
//{
//	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
//	//sendToRasterizer.projectedCoordinate.w = 1;
//	
//	//sendToRasterizer.projectedCoordinate.xy = fromVertexBuffer.coordinate.xy;
//		
//	
//	//sendToRasterizer.projectedCoordinate.xy += constantOffset;
//	
//	
//	sendToRasterizer.colorOut = constantColor;
//	
//	sendToRasterizer.projectedCoordinate = mul(input.coordinate, World);
//	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, View);
//	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, Projection);
//	return sendToRasterizer;
//}
#pragma pack_matrix(row_major)
struct INPUT_VERTEX
{
	float3 vertex           : POSITION;
	float3 normal           : NORMAL;
	float3 uv               : TEXCOORD0;
	float4 tangent          : TANGENT;
	float4 weight           : WEIGHT;
	uint4 indices	        : INDEX;
	/*float4 blendWeights 	: BLENDWEIGHT0;
	uint4  boneIndices		: BLENDINDICES0;*/
};

struct OUTPUT_VERTEX
{
	float4 projectedCoordinate	: SV_POSITION;
	float3 normal				: NORMAL;
	float3 uv					: TEXCOORD0;
	float4 WorldPos             : WORLDPOSITION;
};

// TODO: PART 3 STEP 2a
cbuffer CC_VRAM : register(b0)
{
	/*float4 constantColor;
	matrix World;
	matrix View;
	matrix Projection;*/
	float4x4 view;
	float4x4 proj;
	float4x4 viewproj;
};

cbuffer TRANS_VRAM : register(b1)
{
	float4x4 trans;
};

cbuffer cbChangesEveryFrame : register(b2) 
{
	float4x4 BoneOffset[4];
};

OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;

	float4x4 finalPos   = BoneOffset[0] * input.weight[0]
						+ BoneOffset[1] * input.weight[1]
						+ BoneOffset[2] * input.weight[2]
						+ BoneOffset[3] * input.weight[3];

	float4 coordinate = mul(float4(input.vertex,1), finalPos);
	coordinate = mul(coordinate, trans);
	output.WorldPos = coordinate;

	output.projectedCoordinate = mul(coordinate, viewproj);
	output.normal = mul(input.normal, (float3x3)trans);
	output.uv = input.uv;
	//sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, View);
	//sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, Projection);
	return output;
}