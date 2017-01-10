
#include "PrimitiveObjs.h"

class DEMO_APP
{	
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	
	
	IDXGISwapChain *theSwapChain = 0;
	ID3D11RenderTargetView *theRTV = 0;
	
	D3D11_VIEWPORT theViewPort;
	ID3D11View *theview = 0;
	ID3D11Debug *theDebug = 0;
	ID3D11Texture2D *textBuff = 0;
	HRESULT hr;

	
	
	theMesh plane;

public:
	
	
	SIMPLE_VERTEX velocity;
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	
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
	
	
	plane.PlaneSetUp();
}


bool DEMO_APP::Run()
{
	
	 theContext->OMSetRenderTargets(1, &theRTV, NULL);
	
	theContext->RSSetViewports(1, &theViewPort);
	
	theContext->ClearRenderTargetView(theRTV, Colors::CornflowerBlue);

	plane.RenderPlane();
	
	
	theSwapChain->Present(0, 0);
	return true; 
}

bool DEMO_APP::ShutDown()
{

	SAFE_RELEASE(theSwapChain);
	theDevice->Release();
	theContext->ClearState();
	theDebug->Release();
	SAFE_RELEASE(theview);
	theRTV->Release();
	textBuff->Release();
	plane.release();
	
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
