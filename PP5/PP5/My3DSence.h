#pragma once
#include"Struct.h"
#include "Shape.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
class My3DSence
{
	ComPtr<ID3D11Device> theDevice;
	ComPtr<IDXGISwapChain> theSwapChain;
	ComPtr<ID3D11DeviceContext> theContext;

	ComPtr<ID3D11RenderTargetView> theRTV;
	D3D11_VIEWPORT theViewPort;
	ComPtr<ID3D11DepthStencilView> theDSV;
	ComPtr<ID3D11Texture2D> depthBuff;
	
	//ComPtr<ID3D11View> theview;
	//ComPtr<ID3D11Debug> theDebug ;
	ComPtr<ID3D11Texture2D> textBuff;
	HRESULT hr;

	//ID3D11Buffer* g_pConstantBuffer;
	//XMMATRIX g_World;
	//XMMATRIX g_View;
	//XMMATRIX g_Projection;

	//ID3D11Buffer *theVram;
	//ID3D11Buffer *theGridBuffer;

	Shader shader;
	Shape shape;
	Mesh mesh;
	Camera camera;
	XTime time;
	//ID3D11Resource *resource = nullptr;
	//ID3D11Buffer *shadercombuffer = nullptr;
	//ID3D11Buffer *gridConstBuffer = nullptr;
public:
	My3DSence();
	~My3DSence();

	bool Initialize(HWND wnd);
	bool run();
	Camera* GetCamera();
};

