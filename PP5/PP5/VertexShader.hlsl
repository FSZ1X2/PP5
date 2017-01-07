struct INPUT_VERTEX
{
	float2 coordinate : POSITION;
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

cbuffer ConstantBuffer : register(b0)
{

}

OUTPUT_VERTEX main(float4 Pos : POSITION)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	//sendToRasterizer.projectedCoordinate.w = 1;

	//sendToRasterizer.projectedCoordinate.xy = fromVertexBuffer.coordinate.xy;


	//sendToRasterizer.projectedCoordinate.xy += constantOffset;


	sendToRasterizer.colorOut = constantColor;

	sendToRasterizer.projectedCoordinate = mul(Pos, World);
	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, View);
	sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, Projection);
	return sendToRasterizer;
}