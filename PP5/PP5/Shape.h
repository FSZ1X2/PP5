#pragma once
#include "Struct.h"

class Shape
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> constantBuffer;
	XMFLOAT4X4 transform;
	static ID3D11Device* dev;
	static ID3D11DeviceContext* con;

	ComPtr<ID3D11Buffer> lightd;
	ComPtr<ID3D11Buffer> lightp;
	ComPtr<ID3D11Buffer> lights;

	DirectionalLightConstantBuffer dcfd;
	PointLightConstantBuffer pcfd;
	SpotLightConstantBuffer scfd;

	void CreateDirectionalLight();
	void CreatePointLight();
	void CreateSpotLight();
	void UpdataLight(float const moveSpd);

public:
	static void InitDevice(ID3D11Device* _dev, ID3D11DeviceContext* _con);
	void initializeShape(float size = 1);
	void draw();
};

