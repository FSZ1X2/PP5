#pragma once

#include <iostream>
#include <ctime>
#include "XTime.h"


#include <vector>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>


#include "Trivial_VS.csh"
#include "Trivial_PS.csh"

using namespace std;
using namespace DirectX;

#define BACKBUFFER_WIDTH	1000
#define BACKBUFFER_HEIGHT	1000
#define PI		3.14159f

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

ID3D11Device *theDevice = 0;
ID3D11DeviceContext *theContext = 0;