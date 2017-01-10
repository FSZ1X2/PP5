#pragma once
#include"Struct.h"
class Joint
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> constantBuffer;

	ComPtr<ID3D11Buffer> OffsetconstantBuffer;

	XMFLOAT4X4 transform;
	static ID3D11Device* dev;
	static ID3D11DeviceContext* con;
	FBXLoader fbxflie;
	int vertexcount;
	std::vector<VertexPositionUVNormal> pointsforshere;
	std::vector<unsigned int> index;
	BindPosition BindList;
public:
	static void InitDevice(ID3D11Device* _dev, ID3D11DeviceContext* _con);
	void initializeMesh(float size = 1);
	void draw();
	void makesphere(float rad, int slice, int segment);
};

