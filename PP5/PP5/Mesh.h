#pragma once
#include"Struct.h"

class Mesh
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> constantBuffer;
	ComPtr<ID3D11Buffer> scaleBuffer;
	//ComPtr<ID3D11Buffer> poseBuffer;

	XMFLOAT4X4 transform;
	static ID3D11Device* dev;
	static ID3D11DeviceContext* con;

	//FBXLoader fbxflie;

	//HMODULE DLL = LoadLibrary(L"FBXLoader.dll");
	
	//PosList poselist;
	int vertexcount;
	XMFLOAT4 modelsize;
public:
	static void InitDevice(ID3D11Device* _dev, ID3D11DeviceContext* _con);
	//void initializeMesh(FBXExportDATA * fbxflie,float size = 1);
	void initBinaryMesh(const char* path, float size = 1, float x = 0, float y = 0, float z = 0);
	//void setPos(BindPosition *Bindpose);
	void draw();
};

