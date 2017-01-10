#include "Shape.h"

ID3D11Device* Shape::dev = nullptr;
ID3D11DeviceContext* Shape::con = nullptr;


void Shape::CreateDirectionalLight()
{
	dcfd.direction = { -5.0f,2.0f,0.0f,0.0f };
	dcfd.Dcolor = { 0.5f,0.5f,0.5f,0.5f };
}
void Shape::CreatePointLight()
{
	pcfd.Pointpos = { 0.0f,0.0f,-1.0f,0.0f };
	pcfd.Pcolor = { 0.0f,0.0f,1.0f,0.0f };
	pcfd.lightradius = { 0.5f,0.0f,0.0f,0.0f };
}
void Shape::CreateSpotLight()
{
	scfd.Spotpos = { 0.0f,2.0f,0.0f,0.0f };
	scfd.Scolor = { 1.0f,0.0f,0.0f,0.0f };
	scfd.conedir = { 0.0f,-1.0f,0.0f,0.0f };
	scfd.coneratio = { 0.8f,0.0f,0.0f,0.0f };
}

void Shape::UpdataLight(float const moveSpd)
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

void Shape::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

void Shape::initializeShape(float size)
{
	XMStoreFloat4x4(&transform, XMMatrixIdentity()*size);
	transform._44 = 1;
	VertexPositionUVNormal Plane[] =
	{
		{XMFLOAT3(-1, 0.0f, 1),XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT3(-1.0f, 0.0f, 1),XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f) },
		{XMFLOAT3(1, 0.0f, 1),XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT3(-1.0f, 0.0f, 1),XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f) },
		{XMFLOAT3(-1, 0.0f, -1),XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT3(-1.0f, 1.0f, 1),XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f) },
		{XMFLOAT3(1, 0.0f, -1),XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT3(-1.0f, 1.0f, 1),XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f) },
	};

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(Plane);
	desc.StructureByteStride = sizeof(VertexPositionUVNormal);
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = Plane;

	dev->CreateBuffer(&desc, &source, vertexBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4X4);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	dev->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());

	CreateDirectionalLight();
	CreatePointLight();
	CreateSpotLight();

	D3D11_BUFFER_DESC lightdesc = {};
	lightdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightdesc.ByteWidth = sizeof(DirectionalLightConstantBuffer);
	lightdesc.StructureByteStride = 0;
	lightdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightdesc.Usage = D3D11_USAGE_DYNAMIC;
	dev->CreateBuffer(&lightdesc, 0, lightd.GetAddressOf());

	lightdesc.ByteWidth = sizeof(PointLightConstantBuffer);
	dev->CreateBuffer(&lightdesc, 0, lightp.GetAddressOf());

	lightdesc.ByteWidth = sizeof(SpotLightConstantBuffer);
	dev->CreateBuffer(&lightdesc, 0, lights.GetAddressOf());
}

void Shape::draw()
{
	D3D11_MAPPED_SUBRESOURCE maps;
	con->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &transform, sizeof(XMFLOAT4X4));
	con->Unmap(constantBuffer.Get(), 0);

	con->VSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());

	unsigned int stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;
	con->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	UpdataLight(0.005f);
	D3D11_MAPPED_SUBRESOURCE lightmapr = {};
	HRESULT ahr = con->Map(lightd.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightmapr);
	memcpy(lightmapr.pData, &dcfd, sizeof(dcfd));
	//lightmapr.pData = &dcfd;
	con->Unmap(lightd.Get(), 0);

	ahr = con->Map(lightp.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightmapr);
	memcpy(lightmapr.pData, &pcfd, sizeof(pcfd));
	//lightmapr.pData = &pcfd;
	con->Unmap(lightp.Get(), 0);

	ahr = con->Map(lights.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightmapr);
	memcpy(lightmapr.pData, &scfd, sizeof(scfd));
	//lightmapr.pData = &scfd;
	con->Unmap(lights.Get(), 0);

	con->PSSetConstantBuffers(0, 1, lightd.GetAddressOf());
	con->PSSetConstantBuffers(1, 1, lightp.GetAddressOf());
	con->PSSetConstantBuffers(2, 1, lights.GetAddressOf());
	con->Draw(4, 0);
}
