#include "FBXExportDATA.h"
#include "FBXLoader.h"
void FBXExportDATA::LoadFBX(char * path)
{
	FBXLoader::LoadFBX(path, this);
}

void FBXExportDATA::AddVertex(const XMFLOAT3 & v)
{
	Outvertex.push_back(v);
}

void FBXExportDATA::AddNormal(const XMFLOAT3 & n)
{
	Outnormal.push_back(n);
}

void FBXExportDATA::AddUv(const XMFLOAT3 & uv)
{
	Outuv.push_back(uv);
}

void FBXExportDATA::AddTangent(const XMFLOAT4 & t)
{
	Outtangent.push_back(t);
}

void FBXExportDATA::AddJoint(const XMFLOAT4X4 & v)
{
	OutJoint.push_back(v);
}

const XMFLOAT3 * FBXExportDATA::GetVertex() const
{
	return Outvertex.data();
}

const XMFLOAT3 * FBXExportDATA::GetNormal() const
{
	return Outnormal.data();
}

const XMFLOAT3 * FBXExportDATA::GetUv() const
{
	return Outuv.data();
}

const XMFLOAT4 * FBXExportDATA::GetTangent() const
{
	return Outtangent.data();
}

const XMFLOAT4X4 * FBXExportDATA::GetJoint() const
{
	return OutJoint.data();
}

UINT FBXExportDATA::GetVertexSize() const
{
	return Outvertex.size();
}

UINT FBXExportDATA::GetNormalSize() const
{
	return Outnormal.size();
}

UINT FBXExportDATA::GetUvSize() const
{
	return Outuv.size();
}

UINT FBXExportDATA::GetTangentSize() const
{
	return Outtangent.size();
}

UINT FBXExportDATA::GetJointSize() const
{
	return OutJoint.size();
}
