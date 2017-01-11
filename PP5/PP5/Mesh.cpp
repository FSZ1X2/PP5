#include "Mesh.h"
ID3D11Device* Mesh::dev = nullptr;
ID3D11DeviceContext* Mesh::con = nullptr;

void Mesh::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

void Mesh::initializeMesh(float size)
{
	XMStoreFloat4x4(&transform, XMMatrixIdentity()*size);
	fbxflie.LoadFBX("Box_Attack.fbx");
	//transform = fbxflie.transL;
	unsigned int num = fbxflie.GetVertexSize();
	std::vector<VertexPositionUVNormal> TriangleVertexList;

	for (unsigned int i = 0; i < num; i++)
	{
		VertexPositionUVNormal vertex1;
		vertex1.pos.x = fbxflie.GetVertex()[i].v[0].vertex.x;
		vertex1.pos.y = fbxflie.GetVertex()[i].v[0].vertex.y;
		vertex1.pos.z = fbxflie.GetVertex()[i].v[0].vertex.z;
		vertex1.normal.x = fbxflie.GetVertex()[i].v[0].normal.x;
		vertex1.normal.y = fbxflie.GetVertex()[i].v[0].normal.y;
		vertex1.normal.z = fbxflie.GetVertex()[i].v[0].normal.z;
		vertex1.uv.x = fbxflie.GetVertex()[i].v[0].uv.x;
		vertex1.uv.y = fbxflie.GetVertex()[i].v[0].uv.y;
		vertex1.uv.z = 1.0f;
		vertex1.tangent.x = fbxflie.GetVertex()[i].v[0].tangent.x;
		vertex1.tangent.y = fbxflie.GetVertex()[i].v[0].tangent.y;
		vertex1.tangent.z = fbxflie.GetVertex()[i].v[0].tangent.z;
		vertex1.tangent.w = 0.0f;

		vertex1.blendWeight[0] = 0.25f;
		vertex1.blendWeight[1] = 0.25f;
		vertex1.blendWeight[2] = 0.25f;
		vertex1.blendWeight[3] = 0.25f;

		VertexPositionUVNormal vertex2;
		vertex2.pos.x = fbxflie.GetVertex()[i].v[1].vertex.x;
		vertex2.pos.y = fbxflie.GetVertex()[i].v[1].vertex.y;
		vertex2.pos.z = fbxflie.GetVertex()[i].v[1].vertex.z;
		vertex2.normal.x = fbxflie.GetVertex()[i].v[1].normal.x;
		vertex2.normal.y = fbxflie.GetVertex()[i].v[1].normal.y;
		vertex2.normal.z = fbxflie.GetVertex()[i].v[1].normal.z;
		vertex2.uv.x = fbxflie.GetVertex()[i].v[1].uv.x;
		vertex2.uv.y = fbxflie.GetVertex()[i].v[1].uv.y;
		vertex2.uv.z = 1.0f;
		vertex2.tangent.x = fbxflie.GetVertex()[i].v[1].tangent.x;
		vertex2.tangent.y = fbxflie.GetVertex()[i].v[1].tangent.y;
		vertex2.tangent.z = fbxflie.GetVertex()[i].v[1].tangent.z;
		vertex2.tangent.w = 0.0f;

		vertex2.blendWeight[0] = 0.25f;
		vertex2.blendWeight[1] = 0.25f;
		vertex2.blendWeight[2] = 0.25f;
		vertex2.blendWeight[3] = 0.25f;

		VertexPositionUVNormal vertex3;
		vertex3.pos.x = fbxflie.GetVertex()[i].v[2].vertex.x;
		vertex3.pos.y = fbxflie.GetVertex()[i].v[2].vertex.y;
		vertex3.pos.z = fbxflie.GetVertex()[i].v[2].vertex.z;
		vertex3.normal.x = fbxflie.GetVertex()[i].v[2].normal.x;
		vertex3.normal.y = fbxflie.GetVertex()[i].v[2].normal.y;
		vertex3.normal.z = fbxflie.GetVertex()[i].v[2].normal.z;
		vertex3.uv.x = fbxflie.GetVertex()[i].v[2].uv.x;
		vertex3.uv.y = fbxflie.GetVertex()[i].v[2].uv.y;
		vertex3.uv.z = 1.0f;
		vertex3.tangent.x = fbxflie.GetVertex()[i].v[2].tangent.x;
		vertex3.tangent.y = fbxflie.GetVertex()[i].v[2].tangent.y;
		vertex3.tangent.z = fbxflie.GetVertex()[i].v[2].tangent.z;
		vertex3.tangent.w = 0.0f;

		vertex3.blendWeight[0] = 0.25f;
		vertex3.blendWeight[1] = 0.25f;
		vertex3.blendWeight[2] = 0.25f;
		vertex3.blendWeight[3] = 0.25f;

		TriangleVertexList.push_back(vertex1);
		TriangleVertexList.push_back(vertex2);
		TriangleVertexList.push_back(vertex3);
	}

	vertexcount = TriangleVertexList.size();

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = TriangleVertexList.size() * sizeof(VertexPositionUVNormal);
	desc.StructureByteStride = sizeof(VertexPositionUVNormal);
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
	desc.ByteWidth = sizeof(PosList);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	dev->CreateBuffer(&desc, 0, poseBuffer.GetAddressOf());
}

void Mesh::setPos(BindPosition * Bindpose)
{
	for (int i = 0; i < 4; i++)
	{
		//XMStoreFloat4x4(&poselist.pose[i], XMMatrixIdentity());
		//poselist.pose[i] = Bindpose->pos[i];
		XMStoreFloat4x4(&poselist.pose[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&Bindpose->pos[i])) * XMLoadFloat4x4(&Bindpose->pos[i]));
		//XMMatrixInverse(nullptr, XMLoadFloat4x4(&Bindpose->pos[i]))
	}
}

void Mesh::draw()
{
	D3D11_MAPPED_SUBRESOURCE maps;
	con->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &transform, sizeof(XMFLOAT4X4));
	con->Unmap(constantBuffer.Get(), 0);

	con->VSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());

	con->Map(poseBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &poselist, sizeof(PosList));
	con->Unmap(poseBuffer.Get(), 0);

	con->VSSetConstantBuffers(2, 1, poseBuffer.GetAddressOf());

	unsigned int stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;
	con->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	con->Draw(vertexcount, 0);
}


