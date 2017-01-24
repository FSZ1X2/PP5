#include "My3DSence.h"
#include "DDSTextureLoader.h"

//#include"../FBXLoader/FBXExportDATA.h"

My3DSence::My3DSence()
{
}


My3DSence::~My3DSence()
{
}

bool My3DSence::Initialize(HWND wnd)
{
	//FBXExportDATA boxbbb;
	//boxbbb.LoadFBX("Box_Idle.fbx");
	//FBXExportDATA bearbbb;
	//bearbbb.LoadFBX("Teddy_Run.fbx");

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
	DrawLight::InitDevice(theDevice.Get(), theContext.Get());
	SkyBox::InitDevice(theDevice.Get(), theContext.Get());
	shader.Init();

	//light
	CreateDirectionalLight();
	CreatePointLight();
	CreateSpotLight();

	D3D11_BUFFER_DESC lightdesc = {};
	lightdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightdesc.ByteWidth = sizeof(DirectionalLightConstantBuffer);
	lightdesc.StructureByteStride = 0;
	lightdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightdesc.Usage = D3D11_USAGE_DYNAMIC;
	theDevice->CreateBuffer(&lightdesc, 0, lightd.GetAddressOf());

	lightdesc.ByteWidth = sizeof(PointLightConstantBuffer);
	theDevice->CreateBuffer(&lightdesc, 0, lightp.GetAddressOf());

	lightdesc.ByteWidth = sizeof(SpotLightConstantBuffer);
	theDevice->CreateBuffer(&lightdesc, 0, lights.GetAddressOf());

	lightdesc.ByteWidth = sizeof(XMFLOAT4);
	theDevice->CreateBuffer(&lightdesc, 0, lightpcolor.GetAddressOf());

	lightdesc.ByteWidth = sizeof(XMFLOAT4);
	theDevice->CreateBuffer(&lightdesc, 0, lightscolor.GetAddressOf());

///////////////////////////////////////////////////////////////////////////////////////////
	skybox.initializeShape(1);
	Plight.initializeLigtht();
	Slight.initializeLigtht();
	shape.initializeShape(10);

	bear.initBinaryMesh("Teddy_Run.bin", 0.15f);
	box.initBinaryMesh("Box_Idle.bin");

	joint.initBinaryMesh("Box_Idle.bin");
	bearJoint.initBinaryMesh("Teddy_Run.bin", 0.15f);

	animate1.initializeBinaryAnimation("Box_Idle_Animation.bin");
	bearAni1.initializeBinaryAnimation("Teddy_Run_Animation.bin");
	animate2.initializeBinaryAnimation("Box_Attack_Animation.bin");
	bearAni2.initializeBinaryAnimation("Teddy_Attack1_Animation.bin");

	boxanimation.joint = &joint;
	bearanimation.joint = &bearJoint;

	boxanimation.Clip1 = &animate1;
	boxanimation.Clip2 = &animate2;
	bearanimation.Clip1 = &bearAni1;
	bearanimation.Clip2 = &bearAni2;

	boxanimation.current = boxanimation.Clip1;
	bearanimation.current = bearanimation.Clip1;

	camera.InitCamera();
	camera.SetProjection(camera.DegreeToRadian(75), BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT, 0.01f, 1000.0f);

	CreateDDSTextureFromFile(theDevice.Get(),L"TestCube.dds" ,nullptr, textureV.GetAddressOf());
	CreateDDSTextureFromFile(theDevice.Get(), L"Teddy_D.dds", nullptr, textureB.GetAddressOf());

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
	float dt = (float)time.Delta();
	theContext->OMSetRenderTargets(1, theRTV.GetAddressOf(), theDSV.Get());
	theContext->RSSetViewports(1, &theViewPort);
	theContext->ClearRenderTargetView(theRTV.Get(), Colors::CornflowerBlue);
	theContext->ClearDepthStencilView(theDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	camera.Update(dt);
	ID3D11ShaderResourceView* srv = { nullptr };
	theContext->PSSetShaderResources(0, 1, &srv);

	UpdataLight(0.005f);
	D3D11_MAPPED_SUBRESOURCE lightmapr = {};
	HRESULT ahr = theContext->Map(lightd.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightmapr);
	memcpy(lightmapr.pData, &dcfd, sizeof(dcfd));
	theContext->Unmap(lightd.Get(), 0);

	ahr = theContext->Map(lightp.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightmapr);
	memcpy(lightmapr.pData, &pcfd, sizeof(pcfd));
	theContext->Unmap(lightp.Get(), 0);

	ahr = theContext->Map(lights.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightmapr);
	memcpy(lightmapr.pData, &scfd, sizeof(scfd));
	theContext->Unmap(lights.Get(), 0);
	ID3D11Buffer* cbs[] = { lightd.Get() , lightp.Get() , lights.Get() };
	theContext->PSSetConstantBuffers(0, 3, cbs);

	shader.SetLightShader();

	ahr = theContext->Map(lightpcolor.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightmapr);
	memcpy(lightmapr.pData, &pcfd.Pcolor, sizeof(pcfd.Pcolor));
	theContext->Unmap(lightpcolor.Get(), 0);
	theContext->PSSetConstantBuffers(3, 1, lightpcolor.GetAddressOf());
	Plight.TransModel(pcfd.Pointpos.x, pcfd.Pointpos.y, pcfd.Pointpos.z);
	Plight.draw(); 

	ahr = theContext->Map(lightscolor.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightmapr);
	memcpy(lightmapr.pData, &scfd.Scolor, sizeof(scfd.Scolor));
	theContext->Unmap(lightscolor.Get(), 0);
	theContext->PSSetConstantBuffers(3, 1, lightscolor.GetAddressOf());
	Slight.TransModel(scfd.Spotpos.x, scfd.Spotpos.y, scfd.Spotpos.z);
	Slight.draw();

	shader.SetCommonShader();	
	shape.draw();

	theContext->PSSetSamplers(0, 1, binsample.GetAddressOf());
	if (renderBear)
	{
		theContext->PSSetShaderResources(0, 1, textureB.GetAddressOf());
		if (isLoopAnimation)
		{
			if (bearanimation.ifBlend)
			{
				if (bearanimation.current == bearanimation.Clip1)
					bearanimation.BlendAnimation(dt*0.5f, bearanimation.Clip2);
				else							   
					bearanimation.BlendAnimation(dt*0.5f, bearanimation.Clip1);
			}
			else
				bearanimation.Interpolate(dt*0.5f, bearanimation.current);
			bearanimation.setJoint();
			//bearAni.Interpolate(dt*0.5f);
		}
		else
		{
			if (frameBear < bearanimation.current->GetTotalKeyframes())
			{
				bearanimation.sentToJoint(frameBear, bearanimation.current);
				bearanimation.setJoint();
				//bearAni.sentToJoint(frameBear);
			}
			else
				frameBear = 0;
		}
		shader.SetCommonShader();
		bearJoint.draw(drawBone);

		shader.SetGroundShader();
		bear.draw(drawMesh);
	}
	else
	{
		theContext->PSSetShaderResources(0, 1, textureV.GetAddressOf());
		if (isLoopAnimation)
		{
			if (boxanimation.ifBlend)
			{
				if (boxanimation.current == boxanimation.Clip1)
					boxanimation.BlendAnimation(dt*0.5f, boxanimation.Clip2);
				else								 
					boxanimation.BlendAnimation(dt*0.5f, boxanimation.Clip1);
			}
			else
				boxanimation.Interpolate(dt*0.5f, boxanimation.current);
			boxanimation.setJoint();
			//animate.Interpolate(dt);
		}
		else
		{
			if (frameBox < boxanimation.current->GetTotalKeyframes())
			{
				boxanimation.sentToJoint(frameBox, boxanimation.current);
				boxanimation.setJoint();
				//animate.sentToJoint(frameBox);
			}
			else
				frameBox = 0;
		}
		shader.SetCommonShader();
		joint.draw(drawBone);

		shader.SetGroundShader();
		box.draw(drawMesh);
	}
	if (GetAsyncKeyState('5') & 0x1)
	{
		if (renderBear)
		{
			if (bearanimation.ifBlend)
			{
				bearanimation.current->currtime = 0.0f;
				if (bearanimation.current == bearanimation.Clip1)
					bearanimation.current = bearanimation.Clip2;
				else
					bearanimation.current = bearanimation.Clip1;
			}
			bearanimation.ifBlend = true;
			bearanimation.timer = 0.0f;
		}
		else
		{
			if (boxanimation.ifBlend)
			{
				boxanimation.current->currtime = 0.0f;
				if (boxanimation.current == boxanimation.Clip1)
					boxanimation.current = boxanimation.Clip2;
				else
					boxanimation.current = boxanimation.Clip1;
			}
			boxanimation.ifBlend = true;
			boxanimation.timer = 0.0f;
		}
	}
	if (GetAsyncKeyState('6') & 0x1)
	{
		drawMesh = !drawMesh;
	}
	if (GetAsyncKeyState('7') & 0x1)
	{
		drawBone = !drawBone;
	}
	if (GetAsyncKeyState('8') & 0x1)
	{
		frameBear++;
		frameBox++;
	}
	if (GetAsyncKeyState('0') & 0x1)
	{
		isLoopAnimation = !isLoopAnimation;
		frameBear = 0;
		frameBox = 0;
	}
	if (GetAsyncKeyState('9')&0x1)
	{
		renderBear = !renderBear;
		frameBear = 0;
		frameBox = 0;
	}
	shader.SetSkyBoxShader();
	XMFLOAT4X4 camPos;
	XMStoreFloat4x4(&camPos, camera.GetPos());
	skybox.draw(camPos._41, camPos._42, camPos._43);

	theSwapChain->Present(0, 0);
	time.Signal();
	return true;
}

Camera * My3DSence::GetCamera()
{
	return &camera;
}

//light stuff:
void My3DSence::CreateDirectionalLight()
{
	dcfd.direction = { -5.0f,-0.0f,1.0f,0.0f };
	dcfd.Dcolor = { 1.0f,1.0f,1.0f,1.0f };
}
void My3DSence::CreatePointLight()
{
	pcfd.Pointpos = { -3.3f,0.8f,-1.0f,1.0f };
	pcfd.Pcolor = { 0.0f,1.0f,1.0f,0.0f };
	pcfd.lightradius = { 5.0f,0.0f,0.0f,0.0f };
}
void My3DSence::CreateSpotLight()
{
	scfd.Spotpos = { 2.6f,0.9f,0.0f,1.0f };
	scfd.Scolor = { 1.0f,0.0f,0.0f,0.0f };
	scfd.conedir = { 0.0f,-1.0f,0.0f,0.0f };
	scfd.coneratio = { 0.8f,0.9f,0.0f,0.0f };
}
void My3DSence::UpdataLight(float const moveSpd)
{
	//directional:
	if (GetAsyncKeyState('I') && GetAsyncKeyState('1'))
	{
		dcfd.direction.z += moveSpd;
	}
	if (GetAsyncKeyState('K') && GetAsyncKeyState('1'))
	{
		dcfd.direction.z += -moveSpd;
	}
	if (GetAsyncKeyState('J') && GetAsyncKeyState('1'))
	{
		dcfd.direction.x += -moveSpd;
	}
	if (GetAsyncKeyState('L') && GetAsyncKeyState('1'))
	{
		dcfd.direction.x += moveSpd;
	}
	if (GetAsyncKeyState('U') && GetAsyncKeyState('1'))
	{
		dcfd.direction.y += -moveSpd;
	}
	if (GetAsyncKeyState('O') && GetAsyncKeyState('1'))
	{
		dcfd.direction.y += moveSpd;
	}

	//pointlight
	if (GetAsyncKeyState('I') && GetAsyncKeyState('2'))
	{
		pcfd.Pointpos.z += moveSpd;
	}
	if (GetAsyncKeyState('K') && GetAsyncKeyState('2'))
	{
		pcfd.Pointpos.z += -moveSpd;
	}
	if (GetAsyncKeyState('J') && GetAsyncKeyState('2'))
	{
		pcfd.Pointpos.x += -moveSpd;
	}
	if (GetAsyncKeyState('L') && GetAsyncKeyState('2'))
	{
		pcfd.Pointpos.x += moveSpd;
	}
	if (GetAsyncKeyState('U') && GetAsyncKeyState('2'))
	{
		pcfd.Pointpos.y += -moveSpd;
	}
	if (GetAsyncKeyState('O') && GetAsyncKeyState('2'))
	{
		pcfd.Pointpos.y += moveSpd;
	}

	//Spotlight pos
	if (GetAsyncKeyState('I') && GetAsyncKeyState('3'))
	{
		scfd.Spotpos.z += moveSpd;
	}
	if (GetAsyncKeyState('K') && GetAsyncKeyState('3'))
	{
		scfd.Spotpos.z += -moveSpd;
	}
	if (GetAsyncKeyState('J') && GetAsyncKeyState('3'))
	{
		scfd.Spotpos.x += -moveSpd;
	}
	if (GetAsyncKeyState('L') && GetAsyncKeyState('3'))
	{
		scfd.Spotpos.x += moveSpd;
	}
	if (GetAsyncKeyState('U') && GetAsyncKeyState('3'))
	{
		scfd.Spotpos.y += -moveSpd;
	}
	if (GetAsyncKeyState('O') && GetAsyncKeyState('3'))
	{
		scfd.Spotpos.y += moveSpd;
	}

	//Spotlight dir
	if (GetAsyncKeyState('I') && GetAsyncKeyState('4'))
	{
		scfd.conedir.z += moveSpd;
	}
	if (GetAsyncKeyState('K') && GetAsyncKeyState('4'))
	{
		scfd.conedir.z += -moveSpd;
	}
	if (GetAsyncKeyState('J') && GetAsyncKeyState('4'))
	{
		scfd.conedir.x += -moveSpd;
	}
	if (GetAsyncKeyState('L') && GetAsyncKeyState('4'))
	{
		scfd.conedir.x += moveSpd;
	}
	if (GetAsyncKeyState('U') && GetAsyncKeyState('4'))
	{
		scfd.conedir.y += -moveSpd;
	}
	if (GetAsyncKeyState('O') && GetAsyncKeyState('4'))
	{
		scfd.conedir.y += moveSpd;
	}
}

