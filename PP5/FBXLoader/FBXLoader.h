#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include<fbxsdk.h>
#include <vector>
#include <assert.h>
using namespace DirectX;

#define FBX_API __declspec(dllexport)

namespace fbxloader
{
	class FBX_API FBXLoader
	{
	public:
		struct FBXVertex
		{
			XMFLOAT3 vertex;
			XMFLOAT3 normal;
			XMFLOAT3 uv;
			XMFLOAT4 tangent;		
		};

		struct FBXTriangle
		{
			FBXVertex v[3];
		};

		struct FBXJoint
		{
			FbxMatrix bindposinverse;
		};	
	private:
		FbxManager* FbxSdkManager = nullptr;
		std::vector<FBXTriangle> pOutVertexVector;
		std::vector<FBXJoint> pOutJoint;
		XMFLOAT4X4 transL;
	public:
		void LoadFBX(char* path);
		inline FBXTriangle* GetVertex() { return pOutVertexVector.data(); }
		inline UINT GetVertexSize() { return pOutVertexVector.size(); }
		inline FBXJoint* GetJoint() { return pOutJoint.data(); }
		inline UINT GetJointSize() { return pOutJoint.size(); }
	private:
		void ProcessNode(FbxNode* pNode);
		void ProcessMesh(FbxNode* pNode);
		void ProcessSkeleton(FbxNode* pNode);
		void ReadVertex(FbxMesh* pMesh, int ctrlPointIndex, XMFLOAT3* pVertex);
		void ReadColor(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT4* pColor);
		void ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, XMFLOAT3* pUV);
		void ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT3* pNormal);
		void ReadTangent(FbxMesh* pMesh, int ctrlPointIndex, int vertecCounter, XMFLOAT4* pTangent);

	};
}
