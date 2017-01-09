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

struct INPUT_VERTEX
{
	float3 vertex: POSITION;
	float4 color: COLOR;
	float3 normal: NORMAL;
	float3 tangent: TANGENT;
	float2 uv: UV;
};

struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
};

// TODO: PART 3 STEP 2a
cbuffer THIS_IS_VRAM : register(b0)
{
	float4 constantColor;
	matrix World;
	matrix View;
	matrix Projection;
};


OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;

	sendToRasterizer.colorOut = constantColor;
	float4 coordinate = float4(input.vertex, 1.0f);

	sendToRasterizer.projectedCoordinate = mul(coordinate, World);
	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, View);
	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, Projection);
	return sendToRasterizer;
}