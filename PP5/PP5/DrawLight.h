#pragma once
#include"Struct.h"
class DrawLight
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> constantBuffer;
	XMFLOAT4X4 transform;
	static ID3D11Device* dev;
	static ID3D11DeviceContext* con;
public:
	static void InitDevice(ID3D11Device* _dev, ID3D11DeviceContext* _con);
	void initializeLigtht(XMFLOAT4 pos, float size = 1);
	void draw();
	void TransModel(float x, float y, float z);
};

