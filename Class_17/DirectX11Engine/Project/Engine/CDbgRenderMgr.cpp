#include "pch.h"
#include "CDbgRenderMgr.h"

#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

CDbgRenderMgr::CDbgRenderMgr()
	: m_DebugRenderObj(nullptr)
{
	m_DebugRenderObj = new CGameObject;
	m_DebugRenderObj->AddComponent(new CTransform);
	m_DebugRenderObj->AddComponent(new CMeshRender);
	m_DebugRenderObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));	
}

CDbgRenderMgr::~CDbgRenderMgr()
{
	delete m_DebugRenderObj;
	m_DebugRenderObj = nullptr;
}

void CDbgRenderMgr::render()
{
	list<tDebugShapeInfo>::iterator iter = m_ShapeInfo.begin();

	for (; iter != m_ShapeInfo.end(); )
	{
		// 월드행렬 연산이 안되어있으면 연산한다.
		if (iter->matWorld == XMMatrixIdentity())
		{
			m_DebugRenderObj->Transform()->SetRelativePos(iter->Position);
			m_DebugRenderObj->Transform()->SetRelativeScale(iter->Scale);
			m_DebugRenderObj->Transform()->SetRelativeRotation(iter->Rotation);
			m_DebugRenderObj->Transform()->finaltick();
		}
		else
		{
			m_DebugRenderObj->Transform()->SetWorldMat(iter->matWorld);
		}


		// 메쉬 참조
		switch (iter->Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugRenderObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetRSType(RS_TYPE::CULL_NONE);
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugRenderObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetRSType(RS_TYPE::CULL_NONE);
			break;
		case DEBUG_SHAPE::LINE:
			m_DebugRenderObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"LineMesh"));
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetRSType(RS_TYPE::CULL_BACK);
			break;
		case DEBUG_SHAPE::CUBE:
			m_DebugRenderObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh_Debug"));
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetRSType(RS_TYPE::CULL_NONE);
			break;
		case DEBUG_SHAPE::SPHERE:
			m_DebugRenderObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetRSType(RS_TYPE::CULL_FRONT);
			break;
		default:
			break;
		}

		// Material Binding
		m_DebugRenderObj->MeshRender()->GetMaterial()->SetScalarParam(INT_0, iter->Shape);
		m_DebugRenderObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, iter->Color);

		// DepthTest
		if (iter->DepthTest)
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_DebugRenderObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// render
		m_DebugRenderObj->render();


		// Life Check
		iter->Age += DT_ENGINE;

		if (iter->Duration <= iter->Age)
		{
			iter = m_ShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}