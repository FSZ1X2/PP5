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

void FBXExportDATA::AddWeight(const float & v)
{
	OutWeight.push_back(v);
}

void FBXExportDATA::AddBoneIndex(const int & v)
{
	OutBoneIndex.push_back(v);
}

void FBXExportDATA::SetFrameRate(const float & v)
{
	framerate = v;
	framerate_inv = 1.0f / v;
}

void FBXExportDATA::SetTotalTime(const float & v)
{
	animationTime = v;
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

const float * FBXExportDATA::GetWeight() const
{
	return OutWeight.data();
}

const int * FBXExportDATA::GetIndex() const
{
	return OutBoneIndex.data();
}

const float FBXExportDATA::GetAnimationTime() const
{
	return animationTime;
}

const float FBXExportDATA::GetFrameRate() const
{
	return framerate;
}

const float FBXExportDATA::GetFrameRate_Inv() const
{
	return framerate_inv;
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

UINT FBXExportDATA::GetWeightSize() const
{
	return OutWeight.size();
}

UINT FBXExportDATA::GetIndexSize() const
{
	return OutBoneIndex.size();
}
