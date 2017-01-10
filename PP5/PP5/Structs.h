#pragma once

#include "Headers.h"
struct SEND_TO_VRAM
{
	XMVECTORF32 constantColor;
	XMFLOAT4X4 World;
	XMFLOAT4X4 View;
	XMFLOAT4X4 Projection;
	XMFLOAT4 CameraWorldPos;
};

struct SIMPLE_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT3 uv;
	XMFLOAT3 normal;
	XMFLOAT4 tangent;
};

struct DirectionalLightConstantBuffer
{
	XMFLOAT4 direction;
	XMFLOAT4 Dcolor;
};

struct PointLightConstantBuffer
{
	XMFLOAT4 Pointpos;
	XMFLOAT4 Pcolor;
	XMFLOAT4 lightradius;
};

struct SpotLightConstantBuffer
{
	XMFLOAT4 Spotpos;
	XMFLOAT4 Scolor;
	XMFLOAT4 conedir;
	XMFLOAT4 coneratio;
};