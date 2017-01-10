#pragma once

#include "Structs.h"
#include "Lights.h"
struct theMesh
{
	struct meshStuff
	{
		ID3D11Buffer *theVram = 0;
		ID3D11VertexShader *vertexShader;
		ID3D11PixelShader *pixelShader;
		ID3D11InputLayout *layout;
		ID3D11Buffer *shadercombuffer;
		XMFLOAT4X4 camera;
		SEND_TO_VRAM toShader;
	};

	meshStuff mesh;
	theLight light;
	

	void PlaneSetUp()
	{
		D3D11_INPUT_ELEMENT_DESC elements[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};
		theDevice->CreateInputLayout(elements, 4, Trivial_VS, sizeof(Trivial_VS), &mesh.layout);
		theContext->IASetInputLayout(mesh.layout);

		float size = 1.0f;
		float norm = 0.707106769;
		SIMPLE_VERTEX Plane[] =
		{
			{XMFLOAT3(-size, 0.0f, size),  XMFLOAT3(-size, 0.0f, size), XMFLOAT3(-norm, 0.0f, norm) }, //top left
			{ XMFLOAT3(size, 0.0f,  size), XMFLOAT3(size, 0.0f,  size), XMFLOAT3(norm, 0.0f,  norm) }, // top right
			{ XMFLOAT3(-size, 0.0f,-size), XMFLOAT3(-size, 0.0f,-size), XMFLOAT3(-norm, 0.0f,-norm) }, // bot left
			{ XMFLOAT3(size, 0.0f,  size), XMFLOAT3(size, 0.0f,  size), XMFLOAT3(norm, 0.0f,  norm) }, //top right
			{ XMFLOAT3(size, 0.0f, -size), XMFLOAT3(size, 0.0f, -size), XMFLOAT3(norm, 0.0f, -norm) }, //bot right 
			{ XMFLOAT3(-size, 0.0f,-size), XMFLOAT3(-size, 0.0f,-size), XMFLOAT3(-norm, 0.0f,-norm) }, // bot left
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

		theDevice->CreateBuffer(&bufferDesc, &sourceData, &mesh.theVram);
		UINT stride = sizeof(SIMPLE_VERTEX);
		UINT offset = 0;
		theContext->IASetVertexBuffers(0, 1, &mesh.theVram, &stride, &offset);

		theDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &mesh.vertexShader);
		theDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &mesh.pixelShader);

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
		XMStoreFloat4x4(&mesh.toShader.World, w);
		XMVECTORF32 eye = { 0.0f, 0.7f, -1.5f, 0.0f };
		XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
		XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
		XMStoreFloat4x4(&mesh.camera, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));	// creates a view matrix that is already inversed
		XMStoreFloat4x4(&mesh.toShader.View, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

		float aspectR = BACKBUFFER_WIDTH / BACKBUFFER_HEIGHT;
		float fov = 70.0f*PI / 180.0f;
		XMMATRIX p = XMMatrixPerspectiveFovLH(fov, aspectR, 0.01f, 100.0f);
		XMStoreFloat4x4(&mesh.toShader.Projection, XMMatrixTranspose(p));
		mesh.toShader.constantColor = Colors::Gray;

		D3D11_SUBRESOURCE_DATA shadersourceData;
		shadersourceData.pSysMem = &mesh.toShader;
		shadersourceData.SysMemPitch = 0;
		shadersourceData.SysMemSlicePitch = 0;

		theDevice->CreateBuffer(&shaderdesc, &shadersourceData, &mesh.shadercombuffer);

		light.SetUp();
	}
	void UpdateCamera()
	{
		float moveSpd = 0.0005f;
		if (GetAsyncKeyState('W'))
		{
			XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, moveSpd);
			XMMATRIX temp_camera = XMLoadFloat4x4(&mesh.camera);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&mesh.camera, result);
			//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
		}
		if (GetAsyncKeyState('S'))
		{
			XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -moveSpd);
			XMMATRIX temp_camera = XMLoadFloat4x4(&mesh.camera);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&mesh.camera, result);
			//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
		}
		if (GetAsyncKeyState('A'))
		{
			XMMATRIX translation = XMMatrixTranslation(-moveSpd, 0.0f, 0.0f);
			XMMATRIX temp_camera = XMLoadFloat4x4(&mesh.camera);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&mesh.camera, result);
			//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
		}
		if (GetAsyncKeyState('D'))
		{
			XMMATRIX translation = XMMatrixTranslation(moveSpd, 0.0f, 0.0f);
			XMMATRIX temp_camera = XMLoadFloat4x4(&mesh.camera);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&mesh.camera, result);
			//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
		}
		if (GetAsyncKeyState(VK_SPACE))
		{
			XMMATRIX translation = XMMatrixTranslation(0.0f, moveSpd, 0.0f);
			XMMATRIX temp_camera = XMLoadFloat4x4(&mesh.camera);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&mesh.camera, result);
			//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
		}
		if (GetAsyncKeyState(VK_SHIFT))
		{
			XMMATRIX translation = XMMatrixTranslation(0.0f, -moveSpd, 0.0f);
			XMMATRIX temp_camera = XMLoadFloat4x4(&mesh.camera);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&mesh.camera, result);
			//XMStoreFloat4x4(&toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&camera))));
		}
	}
	void RenderPlane()
	{
		UpdateCamera();
		mesh.toShader.CameraWorldPos.x = mesh.camera._41;
		mesh.toShader.CameraWorldPos.y = mesh.camera._42;
		mesh.toShader.CameraWorldPos.z = mesh.camera._43;
		light.UpdataLight(0.0005f);
		light.Render();
		XMStoreFloat4x4(&mesh.toShader.View, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&mesh.camera))));
		
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		theContext->Map(mesh.shadercombuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &mesh.toShader, sizeof(mesh.toShader));
		theContext->Unmap(mesh.shadercombuffer, 0);

		theContext->VSSetConstantBuffers(0, 1, &mesh.shadercombuffer);


		theContext->VSSetShader(mesh.vertexShader, NULL, 0);
		theContext->PSSetShader(mesh.pixelShader, NULL, 0);

		theContext->Draw(6, 0);
	}

	void release()
	{
		light.release();
		mesh.theVram->Release();
		mesh.vertexShader->Release();
		mesh.pixelShader->Release();
		mesh.layout->Release();
		mesh.shadercombuffer->Release();
	}
};
