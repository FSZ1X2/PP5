#include"FBXLoader.h"

namespace fbxloader
{
	void FBXLoader::LoadFBX(char* path)
	{
		if (FbxSdkManager == nullptr)
		{
			FbxSdkManager = FbxManager::Create();

			FbxIOSettings* pIOsettings = FbxIOSettings::Create(FbxSdkManager, IOSROOT);
			FbxSdkManager->SetIOSettings(pIOsettings);
		}

		FbxImporter* pImporter = FbxImporter::Create(FbxSdkManager, "");
		FbxScene* pFbxScene = FbxScene::Create(FbxSdkManager, "");

		bool bSuccess = pImporter->Initialize(path, -1, FbxSdkManager->GetIOSettings());
		bSuccess = pImporter->Import(pFbxScene);
		pImporter->Destroy();

		FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

		if (pFbxRootNode)
		{
			ProcessNode(pFbxRootNode);
		}
	}

	void FBXLoader::ProcessNode(FbxNode* pNode)
	{
		FbxNodeAttribute::EType attributeType;

		if (pNode->GetNodeAttribute())
		{
			switch (pNode->GetNodeAttribute()->GetAttributeType())
			{
			case FbxNodeAttribute::eMesh:
				ProcessMesh(pNode);
				break;
			case FbxNodeAttribute::eSkeleton:
				//ProcessSkeleton(pNode);
				break;
			case FbxNodeAttribute::eLight:
				//ProcessLight(pNode);
				break;
			case FbxNodeAttribute::eCamera:
				//ProcessCamera();
				break;
			}
		}

		for (int i = 0; i < pNode->GetChildCount(); ++i)
		{
			ProcessNode(pNode->GetChild(i));
		}
	}

	void FBXLoader::ProcessMesh(FbxNode* pNode)
	{
		FbxMesh* pMesh = pNode->GetMesh();
		if (pMesh == NULL)
			return;

		int triangleCount = pMesh->GetPolygonCount();
		int vertexCounter = 0;

		for (int i = 0; i < triangleCount; ++i)
		{
			FBXTriangle newTriangle;
			for (int j = 0; j < 3; j++)
			{
				int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

				// Read the vertex  
				ReadVertex(pMesh, ctrlPointIndex, &newTriangle.v[j].vertex);

				// Read the color of each vertex  
				ReadColor(pMesh, ctrlPointIndex, vertexCounter, &newTriangle.v[j].color);

				// Read the UV of each vertex  
				for (int k = 0; k < 2; ++k)
				{
					ReadUV(pMesh, ctrlPointIndex, pMesh->GetTextureUVIndex(i, j), k, &newTriangle.v[j].uv);
				}

				// Read the normal of each vertex  
				ReadNormal(pMesh, ctrlPointIndex, vertexCounter, &newTriangle.v[j].normal);

				// Read the tangent of each vertex  
				ReadTangent(pMesh, ctrlPointIndex, vertexCounter, &newTriangle.v[j].tangent);

				vertexCounter++;
			}

			pOutVertexVector.push_back(newTriangle);
		}
	}

	void FBXLoader::ReadVertex(FbxMesh* pMesh, int ctrlPointIndex, XMFLOAT3* pVertex)
	{
		FbxVector4* pCtrlPoint = pMesh->GetControlPoints();

		pVertex->x = pCtrlPoint[ctrlPointIndex][0];
		pVertex->y = pCtrlPoint[ctrlPointIndex][1];
		pVertex->z = pCtrlPoint[ctrlPointIndex][2];
	}

	void FBXLoader::ReadColor(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT4* pColor)
	{
		if (pMesh->GetElementVertexColorCount() < 1)
			return;

		FbxGeometryElementVertexColor* pVertexColor = pMesh->GetElementVertexColor(0);
		switch (pVertexColor->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (pVertexColor->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				pColor->x = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;
				pColor->y = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
				pColor->z = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
				pColor->w = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int id = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
				pColor->x = (float)pVertexColor->GetDirectArray().GetAt(id).mRed;
				pColor->y = (float)pVertexColor->GetDirectArray().GetAt(id).mGreen;
				pColor->z = (float)pVertexColor->GetDirectArray().GetAt(id).mBlue;
				pColor->w = (float)pVertexColor->GetDirectArray().GetAt(id).mAlpha;
			}
			break;

			default:
				break;
			}
		}
		break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (pVertexColor->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				pColor->x = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mRed;
				pColor->y = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mGreen;
				pColor->z = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mBlue;
				pColor->w = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mAlpha;
			}
			break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int id = pVertexColor->GetIndexArray().GetAt(vertexCounter);
				pColor->x = (float)pVertexColor->GetDirectArray().GetAt(id).mRed;
				pColor->y = (float)pVertexColor->GetDirectArray().GetAt(id).mGreen;
				pColor->z = (float)pVertexColor->GetDirectArray().GetAt(id).mBlue;
				pColor->w = (float)pVertexColor->GetDirectArray().GetAt(id).mAlpha;
			}
			break;
			default:
				break;
			}
		}
		break;
		}
	}

	void FBXLoader::ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, XMFLOAT2* pUV)
	{
		if (uvLayer >= 2 || pMesh->GetElementUVCount() <= uvLayer)
			return;

		FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);

		switch (pVertexUV->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (pVertexUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				pUV->x = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[0];
				pUV->y = pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[1];
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int id = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex);
				pUV->x = pVertexUV->GetDirectArray().GetAt(id)[0];
				pUV->y = pVertexUV->GetDirectArray().GetAt(id)[1];
			}
			break;
			default:
				break;
			}
		}
		break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (pVertexUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			case FbxGeometryElement::eIndexToDirect:
			{
				pUV->x = pVertexUV->GetDirectArray().GetAt(textureUVIndex)[0];
				pUV->y = pVertexUV->GetDirectArray().GetAt(textureUVIndex)[1];
			}
			break;
			default:
				break;
			}
		}
		break;
		}
	}

	void FBXLoader::ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT3* pNormal)
	{
		if (pMesh->GetElementNormalCount() < 1)
			return;

		FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);
		switch (leNormal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (leNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				pNormal->x = leNormal->GetDirectArray().GetAt(ctrlPointIndex)[0];
				pNormal->y = leNormal->GetDirectArray().GetAt(ctrlPointIndex)[1];
				pNormal->z = leNormal->GetDirectArray().GetAt(ctrlPointIndex)[2];
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int id = leNormal->GetIndexArray().GetAt(ctrlPointIndex);
				pNormal->x = leNormal->GetDirectArray().GetAt(id)[0];
				pNormal->y = leNormal->GetDirectArray().GetAt(id)[1];
				pNormal->z = leNormal->GetDirectArray().GetAt(id)[2];
			}
			break;

			default:
				break;
			}
		}
		break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (leNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				pNormal->x = leNormal->GetDirectArray().GetAt(vertexCounter)[0];
				pNormal->y = leNormal->GetDirectArray().GetAt(vertexCounter)[1];
				pNormal->z = leNormal->GetDirectArray().GetAt(vertexCounter)[2];
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int id = leNormal->GetIndexArray().GetAt(vertexCounter);
				pNormal->x = leNormal->GetDirectArray().GetAt(id)[0];
				pNormal->y = leNormal->GetDirectArray().GetAt(id)[1];
				pNormal->z = leNormal->GetDirectArray().GetAt(id)[2];
			}
			break;

			default:
				break;
			}
		}
		break;
		}
	}

	void FBXLoader::ReadTangent(FbxMesh* pMesh, int ctrlPointIndex, int vertecCounter, XMFLOAT3* pTangent)
	{
		if (pMesh->GetElementTangentCount() < 1)
			return;

		FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);

		switch (leTangent->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (leTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				pTangent->x = leTangent->GetDirectArray().GetAt(ctrlPointIndex)[0];
				pTangent->y = leTangent->GetDirectArray().GetAt(ctrlPointIndex)[1];
				pTangent->z = leTangent->GetDirectArray().GetAt(ctrlPointIndex)[2];
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int id = leTangent->GetIndexArray().GetAt(ctrlPointIndex);
				pTangent->x = leTangent->GetDirectArray().GetAt(id)[0];
				pTangent->y = leTangent->GetDirectArray().GetAt(id)[1];
				pTangent->z = leTangent->GetDirectArray().GetAt(id)[2];
			}
			break;

			default:
				break;
			}
		}
		break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (leTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				pTangent->x = leTangent->GetDirectArray().GetAt(vertecCounter)[0];
				pTangent->y = leTangent->GetDirectArray().GetAt(vertecCounter)[1];
				pTangent->z = leTangent->GetDirectArray().GetAt(vertecCounter)[2];
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int id = leTangent->GetIndexArray().GetAt(vertecCounter);
				pTangent->x = leTangent->GetDirectArray().GetAt(id)[0];
				pTangent->y = leTangent->GetDirectArray().GetAt(id)[1];
				pTangent->z = leTangent->GetDirectArray().GetAt(id)[2];
			}
			break;

			default:
				break;
			}
		}
		break;
		}
	}

}
