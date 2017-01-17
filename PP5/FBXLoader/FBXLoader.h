#pragma once
#include <fbxsdk.h>
#include "FBXExportDATA.h"
//#include <d3d11.h>
//#include <DirectXMath.h>
//#include <DirectXCollision.h>
//#include <vector>
//#include <assert.h>
//using namespace DirectX;

//#define EXPORT
//#if defined EXPORT
//#define FBX_API __declspec(dllexport)
//#else
//#define FBX_API __declspec(dllimport)
//#endif
//char* path = "Box_Attack.fbx";

class FBXLoader
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
		XMFLOAT4X4 bindposinverse;
	};	
private:
	//std::vector<FBXTriangle> pOutVertexVector;
	//std::vector<FBXJoint> pOutJoint;
	//XMFLOAT4X4 transL;
public:
	static void LoadFBX(char* path, FBXExportDATA* sdata);
	//FBXTriangle* GetVertex() { return pOutVertexVector.data(); }
	//UINT GetVertexSize() { return pOutVertexVector.size(); }
	//FBXJoint* GetJoint() { return pOutJoint.data(); }
	//UINT GetJointSize() { return pOutJoint.size(); }
private:
	static void ProcessNode(FbxNode* pNode, FBXExportDATA* sdata, FbxScene* pScene);
	static void ProcessMesh(FbxNode* pNode, FBXExportDATA* sdata);
	static void ProcessSkeleton(FbxNode* pNode, FBXExportDATA* sdata);
	static void ProcessKeyframes(FBXExportDATA* sdata, FbxScene* pScene);
	static void ProcessAnimation(FbxNode* pNode, FbxNode* parent,FbxAnimLayer* layer, FBXExportDATA* sdata);
	static void ReadVertex(FbxMesh* pMesh, int ctrlPointIndex, XMFLOAT3* pVertex);
	//void ReadColor(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT4* pColor);
	static void ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, XMFLOAT3* pUV);
	static void ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT3* pNormal);
	static void ReadTangent(FbxMesh* pMesh, int ctrlPointIndex, int vertecCounter, XMFLOAT4* pTangent);

};
	//extern FBX_API FBXLoader fbxflie;

