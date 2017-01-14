#pragma once
#include"Struct.h"
#include "Shape.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Joint.h"
#include "SkyBox.h"
class My3DSence
{
	ComPtr<ID3D11Device> theDevice;
	ComPtr<IDXGISwapChain> theSwapChain;
	ComPtr<ID3D11DeviceContext> theContext;

	ComPtr<ID3D11RenderTargetView> theRTV;
	D3D11_VIEWPORT theViewPort;
	ComPtr<ID3D11DepthStencilView> theDSV;
	ComPtr<ID3D11Texture2D> depthBuff;
	ComPtr<ID3D11ShaderResourceView> textureV;
	//ComPtr<ID3D11Texture2D> texture;
	ComPtr<ID3D11SamplerState> binsample;
	
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

	ComPtr<ID3D11Buffer> lightd;
	ComPtr<ID3D11Buffer> lightp;
	ComPtr<ID3D11Buffer> lights;

	DirectionalLightConstantBuffer dcfd;
	PointLightConstantBuffer pcfd;
	SpotLightConstantBuffer scfd;

	Shader shader;
	Shape shape;
	Mesh mesh;
	Joint joint;
	Camera camera;
	XTime time;
	SkyBox skybox;
	//ID3D11Resource *resource = nullptr;
	//ID3D11Buffer *shadercombuffer = nullptr;
	//ID3D11Buffer *gridConstBuffer = nullptr;

	//FBXExportDATA fbxflie;

	void CreateDirectionalLight();
	void CreatePointLight();
	void CreateSpotLight();
	void UpdataLight(float const moveSpd);
public:
	My3DSence();
	~My3DSence();

	bool Initialize(HWND wnd);
	bool run();
	Camera* GetCamera();
};

