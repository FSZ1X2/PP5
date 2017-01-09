struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
};

struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
};

cbuffer THIS_IS_VRAM : register( b0 )
{
	float4 constantColor;
	matrix World;
	matrix View;
	matrix Projection;
};



OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX sendToRasterizer;
	
	sendToRasterizer.colorOut = constantColor;

	input.coordinate = mul(input.coordinate, World);
	input.coordinate = mul(input.coordinate, View);
	input.coordinate = mul(input.coordinate, Projection);
	sendToRasterizer.projectedCoordinate = input.coordinate;
	return sendToRasterizer;
}