#include "Mesh.h"

ID3D11Device* Mesh::dev = nullptr;
ID3D11DeviceContext* Mesh::con = nullptr;

void Mesh::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

void Mesh::initializeMesh(FBXExportDATA * fbxflie, float size)
{
	XMStoreFloat4x4(&transform, XMMatrixIdentity()*size);
	transform._44 = 1;
	//fbxflie.LoadFBX("Box_Attack.fbx");
	//fbxloader::FBXLoader::LoadFBX("Box_Attack.fbx");
	//transform = fbxflie.transL;
	unsigned int num = fbxflie->GetVertexSize();
	XMMATRIX mFix = XMMatrixSet(-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1);
	std::vector<VertexPositionUVNormal> TriangleVertexList;

	for (unsigned int i = 0; i < num; i++)
	{
		VertexPositionUVNormal vertex1;
		vertex1.pos = fbxflie->GetVertex()[i];
		//vertex1.pos.x *= -1;

		vertex1.normal = fbxflie->GetNormal()[i];
		//vertex1.normal.x *= -1;

		vertex1.uv = fbxflie->GetUv()[i];

		vertex1.tangent = fbxflie->GetTangent()[i];

		memcpy(vertex1.blendIndices, &fbxflie->GetIndex()[i], sizeof(XMINT4));
		memcpy(vertex1.blendWeight, &fbxflie->GetWeight()[i], sizeof(XMFLOAT4));

		TriangleVertexList.push_back(vertex1);
	}

	vertexcount = TriangleVertexList.size();

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = vertexcount * sizeof(VertexPositionUVNormal);
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = &TriangleVertexList[0];

	dev->CreateBuffer(&desc, &source, vertexBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4X4);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	dev->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	dev->CreateBuffer(&desc, 0, scaleBuffer.GetAddressOf());

	modelsize.x = size;
	//D3D11_MAPPED_SUBRESOURCE s;
	//con->Map(scaleBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &s);
	//memcpy(s.pData, &size, sizeof(float));
	//con->Unmap(scaleBuffer.Get(), 0);
	//con->VSSetConstantBuffers(4, 1, scaleBuffer.GetAddressOf());

	//desc.ByteWidth = sizeof(PosList);

	//dev->CreateBuffer(&desc, 0, poseBuffer.GetAddressOf());
}
//
//void Mesh::setPos(BindPosition * Bindpose)
//{
//	for (int i = 0; i < 64; i++)
//	{
//		//XMStoreFloat4x4(&poselist.pose[i], XMMatrixIdentity());
//		//poselist.pose[i] = Bindpose->pos[i];
//		XMStoreFloat4x4(&poselist.pose[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&Bindpose->pos[i])) * XMLoadFloat4x4(&Bindpose->pos[i]));
//		//XMMatrixInverse(nullptr, XMLoadFloat4x4(&Bindpose->pos[i]))
//	}
//}

void Mesh::draw()
{
	D3D11_MAPPED_SUBRESOURCE maps;
	con->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &transform, sizeof(XMFLOAT4X4));
	con->Unmap(constantBuffer.Get(), 0);

	con->VSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());

	con->Map(scaleBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &modelsize, sizeof(float));
	con->Unmap(scaleBuffer.Get(), 0);

	con->VSSetConstantBuffers(4, 1, scaleBuffer.GetAddressOf());

	//con->Map(poseBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	//memcpy(maps.pData, &poselist, sizeof(PosList));
	//con->Unmap(poseBuffer.Get(), 0);

	//con->VSSetConstantBuffers(2, 1, poseBuffer.GetAddressOf());

	unsigned int stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;
	con->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	con->Draw(vertexcount, 0);
}


