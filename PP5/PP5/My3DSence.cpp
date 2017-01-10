#include "My3DSence.h"
#include "DDSTextureLoader.h"
My3DSence::My3DSence()
{
}


My3DSence::~My3DSence()
{
}

bool My3DSence::Initialize(HWND wnd)
{
	time.Restart();
	DXGI_SWAP_CHAIN_DESC description;
	ZeroMemory(&description, sizeof(DXGI_SWAP_CHAIN_DESC));
	description.BufferCount = 1;
	description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	description.BufferDesc.Width = BACKBUFFER_WIDTH;
	description.BufferDesc.Height = BACKBUFFER_HEIGHT;
	description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.OutputWindow = wnd;
	description.SampleDesc.Count = 1;
	description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	description.Windowed = TRUE;

	UINT swapchainFlag = NULL;
#ifdef _DEBUG
	swapchainFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 
		swapchainFlag, NULL, NULL, D3D11_SDK_VERSION, 
		&description, theSwapChain.GetAddressOf(), theDevice.GetAddressOf(), 
		NULL, theContext.GetAddressOf());
	HR(hr);

	hr = theSwapChain->GetBuffer(0, IID_PPV_ARGS(textBuff.GetAddressOf()));
	HR(hr);
	
	hr = theDevice->CreateRenderTargetView(textBuff.Get(), 0, theRTV.GetAddressOf());
	HR(hr);

	D3D11_TEXTURE2D_DESC textdec = {};
	textdec.ArraySize = 1;
	textdec.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textdec.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textdec.Width = BACKBUFFER_WIDTH;
	textdec.Height = BACKBUFFER_HEIGHT;
	textdec.MipLevels = 1;
	textdec.SampleDesc.Count = 1;
	textdec.Usage = D3D11_USAGE_DEFAULT;

	hr = theDevice->CreateTexture2D(&textdec, 0, depthBuff.GetAddressOf());
	HR(hr);
	hr = theDevice->CreateDepthStencilView(depthBuff.Get(), 0, theDSV.GetAddressOf());
	HR(hr);

	ZeroMemory(&theViewPort, sizeof(D3D11_VIEWPORT));
	theViewPort.TopLeftX = 0;
	theViewPort.TopLeftY = 0;
	theViewPort.MaxDepth = 1;
	theViewPort.MinDepth = 0;
	theViewPort.Height = BACKBUFFER_HEIGHT;
	theViewPort.Width = BACKBUFFER_WIDTH;

	theContext->RSSetViewports(1, &theViewPort);

	Shape::InitDevice(theDevice.Get(), theContext.Get());
	Mesh::InitDevice(theDevice.Get(), theContext.Get());
	Shader::InitDevice(theDevice.Get(), theContext.Get());
	Joint::InitDevice(theDevice.Get(), theContext.Get());
	Camera::InitDevice(theDevice.Get(), theContext.Get());

	shader.Init();
	shape.initializeShape(100);
	mesh.initializeMesh();
	joint.initializeMesh();
	camera.InitCamera();
	camera.SetProjection(camera.DegreeToRadian(75), BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT, 0.01f, 1000.0f);

	CreateDDSTextureFromFile(theDevice.Get(),L"TestCube.dds" ,nullptr, textureV.GetAddressOf());

	D3D11_SAMPLER_DESC sdesc = {};
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sdesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sdesc.MaxLOD = D3D11_FLOAT32_MAX;
	sdesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	theDevice->CreateSamplerState(&sdesc, binsample.GetAddressOf());
	return true;
}

bool My3DSence::run()
{
	theContext->OMSetRenderTargets(1, theRTV.GetAddressOf(), theDSV.Get());
	theContext->RSSetViewports(1, &theViewPort);
	theContext->ClearRenderTargetView(theRTV.Get(), Colors::CornflowerBlue);
	theContext->ClearDepthStencilView(theDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	camera.Update((float)time.Delta());
	shader.SetGroundShader();
	ID3D11ShaderResourceView* srv = { nullptr };
	theContext->PSSetShaderResources(0, 1, &srv);
	shape.draw();
	theContext->PSSetShaderResources(0, 1, textureV.GetAddressOf());
	theContext->PSSetSamplers(0, 1, binsample.GetAddressOf());
	mesh.draw();
	joint.draw();
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//hr = theContext->Map(shadercombuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	////memcpy(mappedResource.pData, &toShader, sizeof(toShader));
	//theContext->Unmap(shadercombuffer, 0);

	//theContext->VSSetConstantBuffers(0, 1, &shadercombuffer);

	/*theContext->VSSetShader(vertexShader.Get(), NULL, 0);
	theContext->PSSetShader(pixelShader.Get(), NULL, 0);

	theContext->Draw(6, 0);
*/
	theSwapChain->Present(0, 0);
	time.Signal();
	return true;
}

Camera * My3DSence::GetCamera()
{
	return &camera;
}

