#pragma once
#include <iostream>
#include <ctime>
#include "XTime.h"
using namespace std;
#include <vector>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

#include<wrl.h>
using Microsoft::WRL::ComPtr;
//
//#include "Trivial_VS.csh"
//#include "Trivial_PS.csh"

#include "../FBXLoader/FBXLoader.h"
using namespace fbxloader;

#define BACKBUFFER_WIDTH	1000
#define BACKBUFFER_HEIGHT	1000
#define PI		3.14159f
#define HR(hr)		assert(!FAILED(hr))
// Constant buffer used to send MVP matrices to the vertex shader.
struct CameraConstantBuffer
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4X4 viewProj;
	//XMFLOAT4X4 lightViewMatrix;
	//XMFLOAT4X4 lightProjectionMatrix;
};

struct BindPosition
{
	XMFLOAT4X4 pos[64];
};

struct VertexPositionUVNormal
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 uv;
	DirectX::XMFLOAT4 tangent;
};

struct DirectionalLightConstantBuffer
{
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 Dcolor;
};

struct PointLightConstantBuffer
{
	DirectX::XMFLOAT4 Pointpos;
	DirectX::XMFLOAT4 Pcolor;
	DirectX::XMFLOAT4 lightradius;
};

struct SpotLightConstantBuffer
{
	DirectX::XMFLOAT4 Spotpos;
	DirectX::XMFLOAT4 Scolor;
	DirectX::XMFLOAT4 conedir;
	DirectX::XMFLOAT4 coneratio;
};