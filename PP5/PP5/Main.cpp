// CGS HW Project A "Line Land".
// Author: L.Norri CD CGS, FullSail University

// Introduction:
// Welcome to the hardware project of the Computer Graphics Systems class.
// This assignment is fully guided but still requires significant effort on your part. 
// Future classes will demand the habits & foundation you develop right now!  
// It is CRITICAL that you follow each and every step. ESPECIALLY THE READING!!!

// TO BEGIN: Open the word document that acompanies this project and start from the top.

//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************

#include <iostream>
#include <ctime>
#include "XTime.h"

using namespace std;

// BEGIN PART 1
// TODO: PART 1 STEP 1a
#pragma comment (lib, "d3d11.lib")
#include<d3d11.h>
// TODO: PART 1 STEP 1b
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
using namespace DirectX;
// TODO: PART 2 STEP 6
#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#define BACKBUFFER_WIDTH	1000
#define BACKBUFFER_HEIGHT	1000
#define PI		3.14159f

//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class DEMO_APP
{	
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	// TODO: PART 1 STEP 2
	ID3D11Device *theDevice = 0;
	IDXGISwapChain *theSwapChain = 0;
	ID3D11RenderTargetView *theRTV = 0;
	ID3D11DeviceContext *theContext = 0;
	D3D11_VIEWPORT theViewPort;
	ID3D11View *theview = 0;
	ID3D11Debug *theDebug = 0;
	ID3D11Texture2D *textBuff = 0;
	HRESULT hr;
	
	bool setUp = true;
	// TODO: PART 2 STEP 2
	ID3D11Buffer *theVram = 0;
	unsigned int count = 365;
	ID3D11Buffer *theGridBuffer;
	// BEGIN PART 5
	// TODO: PART 5 STEP 1
	
	// TODO: PART 2 STEP 4
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;
	ID3D11InputLayout *layout;
	ID3D11InputLayout *gridLayout;
	// BEGIN PART 3
	// TODO: PART 3 STEP 1
	ID3D11Resource *resource;
	ID3D11Buffer *shadercombuffer;
	ID3D11Buffer *gridConstBuffer;
	XTime timer;
	double timeStamp;
	// TODO: PART 3 STEP 2b
	struct SEND_TO_VRAM
	{
		XMVECTORF32 constantColor;
		XMFLOAT2 constantOffset;
		XMFLOAT2 padding;
	};
	// TODO: PART 3 STEP 4a
	SEND_TO_VRAM toShader;
	SEND_TO_VRAM gridShader;
public:
	// BEGIN PART 2
	// TODO: PART 2 STEP 1
	struct SIMPLE_VERTEX
	{
		XMFLOAT2 point;
	};
	SIMPLE_VERTEX velocity;
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
};

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
{
	// ****************** BEGIN WARNING ***********************// 
	// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY! 
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

	window = CreateWindow(	L"DirectXApplication", L"",	WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME|WS_MAXIMIZEBOX), 
							CW_USEDEFAULT, CW_USEDEFAULT, window_size.right-window_size.left, window_size.bottom-window_size.top,					
							NULL, NULL,	application, this );												

    ShowWindow( window, SW_SHOW );
	//********************* END WARNING ************************//

	// TODO: PART 1 STEP 3a
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

	// TODO: PART 1 STEP 3b
	UINT swapchainFlag = NULL;
#ifdef _DEBUG
	swapchainFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, swapchainFlag, NULL, NULL, D3D11_SDK_VERSION, &description, &theSwapChain, &theDevice, NULL, &theContext);
	
	// TODO: PART 1 STEP 4
	hr = theSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&textBuff);
	// TODO: PART 1 STEP 5
	hr = theDevice->CreateRenderTargetView(textBuff, NULL, &theRTV);
	hr = theDevice->QueryInterface(__uuidof(ID3D11Debug),(void**)&theDebug);
	ZeroMemory(&theViewPort, sizeof(D3D11_VIEWPORT));
	theViewPort.TopLeftX = 0;
	theViewPort.TopLeftY = 0;
	theViewPort.MaxDepth = 1;
	theViewPort.MinDepth = 0;
	theViewPort.Height = BACKBUFFER_HEIGHT;
	theViewPort.Width = BACKBUFFER_WIDTH;
	// TODO: PART 2 STEP 3a
	SIMPLE_VERTEX circle[365];
	
	for (int i = 0; i < 365; i++)
	{
		circle[i].point.x = sinf(i *PI / 180.0f) * 0.20f;
		circle[i].point.y = cosf(i *PI / 180.0f) * 0.20f;
	}
	// BEGIN PART 4
	// TODO: PART 4 STEP 1

	// TODO: PART 2 STEP 3b
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 365;
	bufferDesc.CPUAccessFlags = NULL;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.MiscFlags = 0;

    // TODO: PART 2 STEP 3c
	D3D11_SUBRESOURCE_DATA sourceData;
	sourceData.pSysMem = circle;
	sourceData.SysMemPitch = 0;
	sourceData.SysMemSlicePitch = 0;
	
	// TODO: PART 2 STEP 3d
	hr = theDevice->CreateBuffer(&bufferDesc, &sourceData, &theVram);
	// TODO: PART 5 STEP 2a
	SIMPLE_VERTEX grid[1200];
	int num = 0;
	for (int y = 10; y > -10; y--)
	{
		int offset = 0;
		if (y % 2 == 0)
			offset = 1;
		for (int x = (-10 + offset) + 1; x <= 9 + offset; x += 2)
		{
			//BotRight
			grid[num].point.x = x / 10.0f;
			grid[num].point.y = (y - 1.0f) / 10.0f;
			num++;
			//BotLeft
			grid[num].point.x = (x - 1.0f) / 10.0f;
			grid[num].point.y = (y - 1.0f) / 10.0f;
			num++;
			//TopLeft
			grid[num].point.x = (x - 1.0f) / 10.0f;
			grid[num].point.y = y / 10.0f;
			num++;
			//TopLeft
			grid[num].point.x = (x - 1.0f) / 10.0f;
			grid[num].point.y = y / 10.0f;
			num++;
			//TopRight			 
			grid[num].point.x = x / 10.0f;
			grid[num].point.y = y / 10.0f;
			num++;
			//BotRight
			grid[num].point.x = x / 10.0f;
			grid[num].point.y = (y - 1.0f) / 10.0f;
			num++;
		}
	}

	// TODO: PART 5 STEP 2b
	D3D11_BUFFER_DESC GridDesc;
	GridDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	GridDesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 1200;
	GridDesc.CPUAccessFlags = NULL;
	GridDesc.Usage = D3D11_USAGE_IMMUTABLE;
	GridDesc.MiscFlags = 0;

	// TODO: PART 2 STEP 3c
	D3D11_SUBRESOURCE_DATA GridData;
	GridData.pSysMem = grid;
	GridData.SysMemPitch = 0;
	GridData.SysMemSlicePitch = 0;

	// TODO: PART 2 STEP 3d
	hr = theDevice->CreateBuffer(&GridDesc, &GridData, &theGridBuffer);
	// TODO: PART 5 STEP 3
		
	// TODO: PART 2 STEP 5
	// ADD SHADERS TO PROJECT, SET BUILD OPTIONS & COMPILE
	
	// TODO: PART 2 STEP 7
	hr = theDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &vertexShader);
	hr = theDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &pixelShader);
	// TODO: PART 2 STEP 8a
	
	D3D11_INPUT_ELEMENT_DESC elements[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		D3D10_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
		

	};
	// TODO: PART 2 STEP 8b
	hr = theDevice->CreateInputLayout(elements, 1, Trivial_VS, sizeof(Trivial_VS), &layout);
	hr = theDevice->CreateInputLayout(elements, 1, Trivial_VS, sizeof(Trivial_VS), &gridLayout);
	// TODO: PART 3 STEP 3

	D3D11_BUFFER_DESC shaderdesc;
	shaderdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shaderdesc.ByteWidth = sizeof(SEND_TO_VRAM) * 1;
	shaderdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shaderdesc.Usage = D3D11_USAGE_DYNAMIC;
	shaderdesc.MiscFlags = 0;

	
	// TODO: PART 3 STEP 4b
	toShader.constantOffset.x = 0;
	toShader.constantOffset.y = 0;
	toShader.constantColor = Colors::Yellow;

	D3D11_SUBRESOURCE_DATA shadersourceData;
	shadersourceData.pSysMem = &toShader;
	shadersourceData.SysMemPitch = 0;
	shadersourceData.SysMemSlicePitch = 0;

	hr = theDevice->CreateBuffer(&shaderdesc, &shadersourceData, &shadercombuffer);

	D3D11_BUFFER_DESC Gridshaderdesc;
	Gridshaderdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Gridshaderdesc.ByteWidth = sizeof(SEND_TO_VRAM) * 1;
	Gridshaderdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Gridshaderdesc.Usage = D3D11_USAGE_DYNAMIC;
	Gridshaderdesc.MiscFlags = 0;


	// TODO: PART 3 STEP 4b
	gridShader.constantOffset.x = 0;
	gridShader.constantOffset.y = 0;
	gridShader.constantColor = Colors::Black;

	D3D11_SUBRESOURCE_DATA gridshadersourceData;
	gridshadersourceData.pSysMem = &toShader;
	gridshadersourceData.SysMemPitch = 0;
	gridshadersourceData.SysMemSlicePitch = 0;

	hr = theDevice->CreateBuffer(&Gridshaderdesc, &gridshadersourceData, &gridConstBuffer);
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{

	 theContext->OMSetRenderTargets(1, &theRTV, NULL);
	
	theContext->RSSetViewports(1, &theViewPort);
	
	theContext->ClearRenderTargetView(theRTV, Colors::CornflowerBlue);

	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	hr = theContext->Map(shadercombuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &toShader, sizeof(toShader));
	theContext->Unmap(shadercombuffer, 0);
	
	theContext->VSSetConstantBuffers(0, 1, &shadercombuffer);
	
	UINT stride = sizeof(SIMPLE_VERTEX);
    UINT offset = 0;
	theContext->IASetVertexBuffers(0,1, &theVram,&stride,&offset);
	
	theContext->VSSetShader(vertexShader, NULL, 0);
	theContext->PSSetShader(pixelShader, NULL, 0);
	
	theContext->IASetInputLayout(layout);
	
	theContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ);
	
	theContext->Draw(365, 0);
	

	
	theSwapChain->Present(0, 0);
	return true; 
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::ShutDown()
{
	// TODO: PART 1 STEP 6
	theSwapChain->Release();
	theDevice->Release();
	theContext->ClearState();
	theDebug->Release();
	theview->Release();
	theRTV->Release();
	textBuff->Release();
	theVram->Release();
	vertexShader->Release();
	pixelShader->Release();
	layout->Release();
	shadercombuffer->Release();
	resource->Release();
	gridConstBuffer->Release();
	gridLayout->Release();
	theGridBuffer->Release();
	/*VramBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();*/
	UnregisterClass( L"DirectXApplication", application ); 
	return true;
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

// ****************** BEGIN WARNING ***********************// 
// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY!
	
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
//********************* END WARNING ************************//