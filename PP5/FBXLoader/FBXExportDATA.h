#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>
#include <assert.h>
using namespace DirectX;
//
#define EXPORT
#if defined EXPORT
#define FBX_API __declspec(dllexport)
#else
#define FBX_API __declspec(dllimport)
#endif
class FBX_API FBXExportDATA
{
private:
	std::vector<XMFLOAT3> Outvertex;
	std::vector<XMFLOAT3> Outnormal;
	std::vector<XMFLOAT3> Outuv;
	std::vector<XMFLOAT4> Outtangent;
	std::vector<XMFLOAT4X4> OutJoint;
	std::vector<XMFLOAT4> OutWeight;
	std::vector<XMINT4> OutBoneIndex;
	float framerate;
	float framerate_inv;
	float animationTime;
	//XMFLOAT4X4 transL;

public:
	void LoadFBX(char* path);

	std::vector<std::vector<XMFLOAT4X4>> keys;
	std::vector<std::vector<float>> keytime;

	void AddVertex(const XMFLOAT3& v);
	void AddNormal(const XMFLOAT3& n);
	void AddUv(const XMFLOAT3& uv);
	void AddTangent(const XMFLOAT4& t);
	void AddJoint(const XMFLOAT4X4& v);
	void AddWeight(const XMFLOAT4& v);
	void AddBoneIndex(const XMINT4& v);
	void SetFrameRate(const float& v);
	void SetTotalTime(const float& v);

	const XMFLOAT3* GetVertex()const;
	const XMFLOAT3* GetNormal()const;
	const XMFLOAT3* GetUv()const;
	const XMFLOAT4* GetTangent()const;
	const XMFLOAT4X4* GetJoint()const;
	const XMFLOAT4* GetWeight()const;
	const XMINT4* GetIndex()const;
	const float GetAnimationTime()const;
	const float GetFrameRate()const;
	const float GetFrameRate_Inv()const;

	UINT GetVertexSize()const;
	UINT GetNormalSize()const;
	UINT GetUvSize()const;
	UINT GetTangentSize()const;
	UINT GetJointSize()const;
	UINT GetWeightSize()const;
	UINT GetIndexSize()const;
};