
#include <iostream>
#include <ctime>
#include "XTime.h"

using namespace std;
#include <vector>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "D3DX11.lib")

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
//#include <D3DX11async.h>
//#include <D3DCompiler.h>
using namespace DirectX;

#include "Trivial_VS.csh"
#include "Trivial_PS.csh"


#define BACKBUFFER_WIDTH	1000
#define BACKBUFFER_HEIGHT	1000
#define PI		3.14159f

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

class DEMO_APP
{	
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	
	ID3D11Device *theDevice = 0;
	IDXGISwapChain *theSwapChain = 0;
	ID3D11RenderTargetView *theRTV = 0;
	ID3D11DeviceContext *theContext = 0;
	D3D11_VIEWPORT theViewPort;
	ID3D11View *theview = 0;
	ID3D11Debug *theDebug = 0;
	ID3D11Texture2D *textBuff = 0;
	HRESULT hr;
	
	ID3D11Buffer* g_pConstantBuffer = NULL;
	XMMATRIX g_World;
	XMMATRIX g_View;
	XMMATRIX g_Projection;

	ID3D11Buffer *theVram = 0;
	unsigned int count = 365;
	ID3D11Buffer *theGridBuffer;

	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;
	ID3D11InputLayout *layout;
	ID3D11InputLayout *gridLayout;

	ID3D11Resource *resource;
	ID3D11Buffer *shadercombuffer;
	ID3D11Buffer *gridConstBuffer;

	XMFLOAT4X4 camera;
	

	struct SEND_TO_VRAM
	{
		XMVECTORF32 constantColor;
		XMFLOAT4X4 World; 
		XMFLOAT4X4 View;
		XMFLOAT4X4 Projection;
	};
	
	SEND_TO_VRAM toShader;
	SEND_TO_VRAM gridShader;
public:
	
	struct SIMPLE_VERTEX
	{
		XMFLOAT4 point;
	};
	SIMPLE_VERTEX velocity;
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	void UpdateCamera();
	bool Run();
	bool ShutDown();
};

DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
{

	application = hinst; 
	appWndProc = proc; 

	WNDCLASSEX  wndClass;
    ZeroMemory( &wndClass, sizeof( wndClass ) );
    wndClass.cbSize         = sizeof( WNDCLASSEX );             
    wndClass.lpfnWndProc    = appWndProc;						
    wndClass.lpszClassName  = L"DirectXApplication";            
	wndClass.hInstance      = application;		               
    wndClass.hCursor        = LoadCursor( NULL, IDC_ARROW );    
    wndClass.hbrBackground  = ( HBRUSH )( COLOR_WINDOWFRAME ); 
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
    RegisterClassEx( &wndClass );

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(	L"DirectXApplication", L"DropOut!!!",	WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME|WS_MAXIMIZEBOX), 
							CW_USEDEFAULT, CW_USEDEFAULT, window_size.right-window_size.left, window_size.bottom-window_size.top,					
							NULL, NULL,	application, this );												

    ShowWindow( window, SW_SHOW );
	

	
	DXGI_SWAP_CHAIN_DESC description;
	ZeroMemory(&description, sizeof(DXGI_SWAP_CHAIN_DESC));
	description.BufferCount = 1;
	description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	description.BufferDesc.Width = BACKBUFFER_WIDTH;
	description.BufferDesc.Height = BACKBUFFER_HEIGHT;
	description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.OutputWindow = window;
	description.SampleDesc.Count = 1;
	description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	description.Windowed = TRUE;

	
	UINT swapchainFlag = NULL;
#ifdef _DEBUG
	swapchainFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, swapchainFlag, NULL, NULL, D3D11_SDK_VERSION, &description, &theSwapChain, &theDevice, NULL, &theContext);
	
	
	hr = theSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&textBuff);
	
	hr = theDevice->CreateRenderTargetView(textBuff, NULL, &theRTV);
	hr = theDevice->QueryInterface(__uuidof(ID3D11Debug),(void**)&theDebug);
	ZeroMemory(&theViewPort, sizeof(D3D11_VIEWPORT));
	theViewPort.TopLeftX = 0;
	theViewPort.TopLeftY = 0;
	theViewPort.MaxDepth = 1;
	theViewPort.MinDepth = 0;
	theViewPort.Height = BACKBUFFER_HEIGHT;
	theViewPort.Width = BACKBUFFER_WIDTH;

	//////////////////////////////////////////////////////////////
	//ID3D10Blob* Vs;
	//ID3D10Blob* Ps;
	//D3DX11CompileFromFile(L"Trivial_VS.hlsl", NULL, NULL, "VS", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, NULL, &Vs, NULL, NULL);
	//D3DX11CompileFromFile(L"Trivial_PS.hlsl", NULL, NULL, "PS", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, NULL, &Ps, NULL, NULL);
	D3D11_INPUT_ELEMENT_DESC elements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		
	};
	hr = theDevice->CreateInputLayout(elements, 1, Trivial_VS, sizeof(Trivial_VS), &layout);
	theContext->IASetInputLayout(layout);
	
	float size = 1.0f;
	
	SIMPLE_VERTEX Plane[] =
	{
		XMFLOAT4(-size, 0.0f, size, 1), //top left
		XMFLOAT4(size, 0.0f, size,  1), // top right
		XMFLOAT4(-size, 0.0f, -size, 1), // bot left
		XMFLOAT4(size, 0.0f, size, 1), //top right
		XMFLOAT4(size, 0.0f, -size,  1), //bot right 
		XMFLOAT4(-size, 0.0f, -size, 1), // bot left
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 6;
	bufferDesc.CPUAccessFlags = NULL;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.MiscFlags = 0;
    
	D3D11_SUBRESOURCE_DATA sourceData;
	sourceData.pSysMem = Plane;
	sourceData.SysMemPitch = 0;
	sourceData.SysMemSlicePitch = 0;
	
	hr = theDevice->CreateBuffer(&bufferDesc, &sourceData, &theVram);
	UINT stride = sizeof(SIMPLE_VERTEX);
    UINT offset = 0;
	theContext->IASetVertexBuffers(0,1, &theVram,&stride,&offset);
	
	hr = theDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &vertexShader);
	hr = theDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &pixelShader);

	theContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_BUFFER_DESC shaderdesc;
	shaderdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shaderdesc.ByteWidth = sizeof(SEND_TO_VRAM) * 1;
	shaderdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shaderdesc.Usage = D3D11_USAGE_DYNAMIC;
	shaderdesc.MiscFlags = 0;

	XMMATRIX w
	  { 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };

	XMMATRIX v;

	w = XMMatrixTranspose(w);
	XMStoreFloat4x4(&toShader.World, w);
	 XMVECTORF32 eye = { 0.0f, 0.7f, -1.5f, 0.0f };
	 XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	 XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	 XMStoreFloat4x4(&camera, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));	// creates a view matrix that is already inversed
	 XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

	float aspectR = BACKBUFFER_WIDTH / BACKBUFFER_HEIGHT;
	float fov = 70.0f*PI / 180.0f;
	XMMATRIX p = XMMatrixPerspectiveFovLH(fov, aspectR, 0.01f, 100.0f);
	XMStoreFloat4x4(&toShader.Projection, XMMatrixTranspose(p));

	toShader.constantColor = Colors::Gray;

	D3D11_SUBRESOURCE_DATA shadersourceData;
	shadersourceData.pSysMem = &toShader;
	shadersourceData.SysMemPitch = 0;
	shadersourceData.SysMemSlicePitch = 0;

	hr = theDevice->CreateBuffer(&shaderdesc, &shadersourceData, &shadercombuffer);

}

void DEMO_APP::UpdateCamera()
{
	float moveSpd = 0.0005f;
	if (GetAsyncKeyState('W'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, moveSpd);
		XMMATRIX temp_camera = XMLoadFloat4x4(&camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&camera, result);
		//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
	}
	if (GetAsyncKeyState('S'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -moveSpd);
		XMMATRIX temp_camera = XMLoadFloat4x4(&camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&camera, result);
		//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
	}
	if (GetAsyncKeyState('A'))
	{
		XMMATRIX translation = XMMatrixTranslation(-moveSpd, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&camera, result);
		//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
	}
	if (GetAsyncKeyState('D'))
	{
		XMMATRIX translation = XMMatrixTranslation(moveSpd, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&camera, result);
		//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, moveSpd, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&camera, result);
		//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
	}
	if (GetAsyncKeyState(VK_SHIFT))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -moveSpd, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&camera, result);
		//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
	}
}

bool DEMO_APP::Run()
{
	UpdateCamera();

	XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
	 theContext->OMSetRenderTargets(1, &theRTV, NULL);
	
	theContext->RSSetViewports(1, &theViewPort);
	
	theContext->ClearRenderTargetView(theRTV, Colors::CornflowerBlue);


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	hr = theContext->Map(shadercombuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &toShader, sizeof(toShader));
	theContext->Unmap(shadercombuffer, 0);
	
	theContext->VSSetConstantBuffers(0, 1, &shadercombuffer);
	
	
	theContext->VSSetShader(vertexShader, NULL, 0);
	theContext->PSSetShader(pixelShader, NULL, 0);
	
	theContext->Draw(6, 0);
	
	theSwapChain->Present(0, 0);
	return true; 
}

bool DEMO_APP::ShutDown()
{

	SAFE_RELEASE(theSwapChain);
	//theSwapChain->Release();
	theDevice->Release();
	theContext->ClearState();
	theDebug->Release();
	SAFE_RELEASE(theview);
	//theview->Release();
	theRTV->Release();
	textBuff->Release();
	theVram->Release();
	vertexShader->Release();
	pixelShader->Release();
	layout->Release();
	shadercombuffer->Release();
	SAFE_RELEASE(resource);
	SAFE_RELEASE(gridConstBuffer);
	SAFE_RELEASE(gridLayout);
	SAFE_RELEASE(theGridBuffer);
	//SAFE_RELEASE(vertexShader);
	//pixelShader->Release();
	UnregisterClass( L"DirectXApplication", application ); 
	return true;
}
	
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,	int nCmdShow );						   
LRESULT CALLBACK WndProc(HWND hWnd,	UINT message, WPARAM wparam, LPARAM lparam );		
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
	srand(unsigned int(time(0)));
	DEMO_APP myApp(hInstance,(WNDPROC)WndProc);	
    MSG msg; ZeroMemory( &msg, sizeof( msg ) );
    while ( msg.message != WM_QUIT && myApp.Run() )
    {	
	    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        { 
            TranslateMessage( &msg );
            DispatchMessage( &msg ); 
        }
    }
	myApp.ShutDown(); 
	return 0; 
}
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if(GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
    switch ( message )
    {
        case ( WM_DESTROY ): { PostQuitMessage( 0 ); }
        break;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
