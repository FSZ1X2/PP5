#pragma once

#ifdef FBX_EXPORTS  
#define FBX_API __declspec(dllexport)   
#else  
#define FBX_API __declspec(dllimport)   
#endif

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <fbxsdk.h>
#include <vector>
#include <assert.h>
using namespace DirectX;

namespace fbxloader
{
	class FBXLoader
	{
		struct FBXVertex
		{
			XMFLOAT3 vertex;
			XMFLOAT4 color;
			XMFLOAT3 normal;
			XMFLOAT3 tangent;
			XMFLOAT2 uv;
		};

		struct FBXTriangle
		{
			FBXVertex v[3];
		};
		
	public:
		FbxManager* FbxSdkManager;
		std::vector<FBXTriangle> pOutVertexVector;
		FBX_API void LoadFBX(char* path);
		void ProcessNode(FbxNode* pNode);
		void ProcessMesh(FbxNode* pNode);
		void ReadVertex(FbxMesh* pMesh, int ctrlPointIndex, XMFLOAT3* pVertex);
		void ReadColor(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT4* pColor);
		void ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, XMFLOAT2* pUV);
		void ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT3* pNormal);
		void ReadTangent(FbxMesh* pMesh, int ctrlPointIndex, int vertecCounter, XMFLOAT3* pTangent);
	};
}