#include "Mesh.h"

ID3D11Device* Mesh::dev = nullptr;
ID3D11DeviceContext* Mesh::con = nullptr;

void Mesh::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}
void Mesh::Calculatenormal(VertexPositionUVNormal &V1, VertexPositionUVNormal &V2, VertexPositionUVNormal &V3)
{
	/*VertexPositionUVNormal vert0 = V1;
	VertexPositionUVNormal vert1 = V2;
	VertexPositionUVNormal vert2 = V3;
	DirectX::XMFLOAT3 vertEdge0;
	vertEdge0.x = vert1.pos.x - vert0.pos.x;
	vertEdge0.y = vert1.pos.y - vert0.pos.y;
	vertEdge0.z = vert1.pos.z - vert0.pos.z;
	DirectX::XMFLOAT3 vertEdge1;
	vertEdge1.x = vert2.pos.x - vert0.pos.x;
	vertEdge1.y = vert2.pos.y - vert0.pos.y;
	vertEdge1.z = vert2.pos.z - vert0.pos.z;

	DirectX::XMFLOAT3 tex0 = vert0.uv;
	DirectX::XMFLOAT3 tex1 = vert1.uv;
	DirectX::XMFLOAT3 tex2 = vert2.uv;

	DirectX::XMFLOAT3 texEdge0;
	texEdge0.x = tex1.x - tex0.x;
	texEdge0.y = tex1.y - tex0.y;
	texEdge0.z = tex1.z - tex0.z;
	DirectX::XMFLOAT3 texEdge1;
	texEdge1.x = tex2.x - tex0.x;
	texEdge1.y = tex2.y - tex0.y;
	texEdge1.z = tex2.z - tex0.z;

	float ratio = 1.0f / (texEdge0.x * texEdge1.y - texEdge1.x * texEdge0.y);
	DirectX::XMFLOAT3 uDirection = DirectX::XMFLOAT3((texEdge1.y * vertEdge0.x - texEdge0.y * vertEdge1.x) * ratio, (texEdge1.y * vertEdge0.y - texEdge0.y * vertEdge1.y) * ratio, (texEdge1.y * vertEdge0.z - texEdge0.y * vertEdge1.z) * ratio);
	DirectX::XMFLOAT3 vDirection = DirectX::XMFLOAT3((texEdge0.x * vertEdge1.x - texEdge1.x * vertEdge0.x) * ratio, (texEdge0.x * vertEdge1.y - texEdge1.x * vertEdge0.y) * ratio, (texEdge0.x * vertEdge1.z - texEdge1.x * vertEdge0.z) * ratio);

	float lenuD = sqrt((uDirection.x * uDirection.x) + (uDirection.y * uDirection.y) + (uDirection.z * uDirection.z));
	uDirection.x = uDirection.x / lenuD;
	uDirection.y = uDirection.y / lenuD;
	uDirection.z = uDirection.z / lenuD;
	float dotResult1 = vert0.normal.x*uDirection.x + vert0.normal.y*uDirection.y + vert0.normal.z*uDirection.z;
	float dotResult2 = vert1.normal.x*uDirection.x + vert1.normal.y*uDirection.y + vert1.normal.z*uDirection.z;
	float dotResult3 = vert2.normal.x*uDirection.x + vert2.normal.y*uDirection.y + vert2.normal.z*uDirection.z;
	DirectX::XMFLOAT4 Tangent1, Tangent2, Tangent3;
	Tangent1.x = uDirection.x - vert0.normal.x * dotResult1;
	Tangent2.x = uDirection.x - vert1.normal.x * dotResult2;
	Tangent3.x = uDirection.x - vert2.normal.x * dotResult3;

	Tangent1.y = uDirection.y - vert0.normal.y * dotResult1;
	Tangent2.y = uDirection.y - vert1.normal.y * dotResult2;
	Tangent3.y = uDirection.y - vert2.normal.y * dotResult3;

	Tangent1.z = uDirection.z - vert0.normal.z * dotResult1;
	Tangent2.z = uDirection.z - vert1.normal.z * dotResult2;
	Tangent3.z = uDirection.z - vert2.normal.z * dotResult3;

	float lenT1 = sqrt((Tangent1.x * Tangent1.x) + (Tangent1.y * Tangent1.y) + (Tangent1.z * Tangent1.z));
	float lenT2 = sqrt((Tangent2.x * Tangent2.x) + (Tangent2.y * Tangent2.y) + (Tangent2.z * Tangent2.z));
	float lenT3 = sqrt((Tangent3.x * Tangent3.x) + (Tangent3.y * Tangent3.y) + (Tangent3.z * Tangent3.z));

	Tangent1.x = Tangent1.x / lenT1;
	Tangent1.y = Tangent1.y / lenT1;
	Tangent1.z = Tangent1.z / lenT1;

	Tangent2.x = Tangent2.x / lenT2;
	Tangent2.y = Tangent2.y / lenT2;
	Tangent2.z = Tangent2.z / lenT2;

	Tangent3.x = Tangent3.x / lenT3;
	Tangent3.y = Tangent3.y / lenT3;
	Tangent3.z = Tangent3.z / lenT3;

	float lenvD = sqrt((vDirection.x * vDirection.x) + (vDirection.y * vDirection.y) + (vDirection.z * vDirection.z));
	vDirection.x = vDirection.x / lenvD;
	vDirection.y = vDirection.y / lenvD;
	vDirection.z = vDirection.z / lenvD;

	DirectX::XMVECTOR cross1 = DirectX::XMVector3Cross(XMLoadFloat3(&vert0.normal), XMLoadFloat3(&uDirection));
	DirectX::XMVECTOR cross2 = DirectX::XMVector3Cross(XMLoadFloat3(&vert1.normal), XMLoadFloat3(&uDirection));
	DirectX::XMVECTOR cross3 = DirectX::XMVector3Cross(XMLoadFloat3(&vert2.normal), XMLoadFloat3(&uDirection));
	DirectX::XMFLOAT3 handedness = vDirection;
	float dotResultv1 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(cross1, XMLoadFloat3(&handedness)));
	float dotResultv2 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(cross2, XMLoadFloat3(&handedness)));
	float dotResultv3 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(cross3, XMLoadFloat3(&handedness)));

	Tangent1.w = (dotResultv1 < 0.0f) ? -1.0f : 1.0f;
	Tangent2.w = (dotResultv2 < 0.0f) ? -1.0f : 1.0f;
	Tangent3.w = (dotResultv3 < 0.0f) ? -1.0f : 1.0f;*/

	XMFLOAT3 dPos1, dPos2, dUV1, dUV2;
	XMFLOAT4 t;
	dPos1.x = V2.pos.x - V1.pos.x;
	dPos1.y = V2.pos.y - V1.pos.y;
	dPos1.z = V2.pos.z - V1.pos.z;

	dPos2.x = V3.pos.x - V1.pos.x;
	dPos2.y = V3.pos.y - V1.pos.y;
	dPos2.z = V3.pos.z - V1.pos.z;

	dUV1.x = V2.uv.x - V1.uv.x;
	dUV1.y = V2.uv.y - V1.uv.y;
	dUV1.z = V2.uv.z - V1.uv.z;

	dUV2.x = V3.uv.x - V1.uv.x;
	dUV2.y = V3.uv.y - V1.uv.y;
	dUV2.z = V3.uv.z - V1.uv.z;

	float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);
	t.x = (dPos1.x * dUV2.y - dPos2.x * dUV1.y)*r;
	t.y = (dPos1.y * dUV2.y - dPos2.y * dUV1.y)*r;
	t.z = (dPos1.z * dUV2.y - dPos2.z * dUV1.y)*r;
	V1.tangent = t;
	V2.tangent = t;
	V3.tangent = t;
}


void Mesh::initBinaryMesh(const char * path, float size, float x, float y, float z)
{
	XMStoreFloat4x4(&transform, XMMatrixIdentity()*size);
	transform._44 = 1;
	ifstream file(path, ios::in | ios::binary | ios::ate);

	file.seekg(0, ios::beg);
	UINT num;
	file.read((char*)&num, sizeof(UINT));
	XMMATRIX mFix = XMMatrixSet(-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1);
	std::vector<VertexPositionUVNormal> TriangleVertexList;

	for (unsigned int i = 0; i < num; i++)
	{
		VertexPositionUVNormal vertex1;
		file.read((char*)&vertex1.pos, sizeof(XMFLOAT3));

		file.read((char*)&vertex1.normal, sizeof(XMFLOAT3));

		file.read((char*)&vertex1.uv, sizeof(XMFLOAT3));

		file.read((char*)&vertex1.tangent, sizeof(XMFLOAT4));
		file.read((char*)&vertex1.blendIndices, sizeof(XMINT4));
		file.read((char*)&vertex1.blendWeight, sizeof(XMFLOAT4));
		

		TriangleVertexList.push_back(vertex1);
	}
	
	file.close();
	vertexcount = TriangleVertexList.size();
	for (int i = 0; i < vertexcount - 3; i = i +3)
	{
		Calculatenormal(TriangleVertexList[i], TriangleVertexList[i + 1], TriangleVertexList[i + 2]);
	}
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
}

void Mesh::draw(bool drawMesh)
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

	unsigned int stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;
	con->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	if (drawMesh)
		con->Draw(vertexcount, 0);
}


