#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CMRT.h"

#include "CAssetMgr.h"
#include "CTexture.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_Info{}
	, m_LightIdx(-1)
	, m_LightCamObj(nullptr)
	, m_ShadowMapMRT(nullptr)
{
	// 광원에서 쉐도우맵 만들때 사용할 카메라
	m_LightCamObj = new CGameObject;
	m_LightCamObj->AddComponent(new CTransform);
	m_LightCamObj->AddComponent(new CCamera);
}

CLight3D::CLight3D(const CLight3D& _Origin)
	: CComponent(_Origin)	
	, m_Info(_Origin.m_Info)
	, m_LightIdx(-1)
	, m_LightCamObj(nullptr)
	, m_ShadowMapMRT(nullptr)
{

}

CLight3D::~CLight3D()
{
	if (nullptr != m_ShadowMapMRT)
		delete m_ShadowMapMRT;

	if (nullptr != m_LightCamObj)
		delete m_LightCamObj;
}

void CLight3D::finaltick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	m_LightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);

	// DebugRender 요청
	if (m_Info.LightType == (UINT)LIGHT_TYPE::POINT)
		DrawDebugSphere(m_Info.WorldPos, m_Info.Range, Vec4(1.f, 1.f, 0.f, 1.f), false, 0.f);
	else
		DrawDebugCube(m_Info.WorldPos, Vec3(50.f, 50.f, 200.f), Transform()->GetRelativeRotation(), Vec4(1.f, 1.f, 0.f, 1.f), true, 0.f);
}

void CLight3D::render_shadowmap()
{
	if (LIGHT_TYPE::DIRECTIONAL != (LIGHT_TYPE)m_Info.LightType)	
		return;

	// ShadowMap MRT 로 교체
	m_ShadowMapMRT->OMSet();
	
	CCamera* pLightCam = m_LightCamObj->Camera();

	g_Trans.matView = pLightCam->GetViewMat();
	g_Trans.matViewInv = pLightCam->GetViewInvMat();
	g_Trans.matProj = pLightCam->GetProjMat();
	g_Trans.matProjInv = pLightCam->GetProjInvMat();

	// Shader Domain 에 따른 물체의 분류작업
	pLightCam->SortObject();
}

void CLight3D::Lighting()
{
	m_LightMtrl->SetScalarParam(INT_0, m_LightIdx);
	m_LightMtrl->Binding();

	if ((LIGHT_TYPE)m_Info.LightType == LIGHT_TYPE::POINT)
	{
		Transform()->Binding();
	}

	m_VolumeMesh->render();
}

void CLight3D::SetRange(float _Range)
{
	m_Info.Range = _Range;
	Transform()->SetRelativeScale(m_Info.Range * 2.f, m_Info.Range * 2.f, m_Info.Range * 2.f);
}

void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.LightType = (UINT)_Type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.LightType)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightMtrl");
		m_LightMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
		m_LightMtrl->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));

		if (m_ShadowMapMRT)
		{
			delete m_ShadowMapMRT;
			m_ShadowMapMRT = new CMRT;

			// 1. ShadowMap Target
			Ptr<CTexture> pShadowMap = CAssetMgr::GetInst()->CreateTexture(L"ShadowMapTargetTex"
										, 4096, 4096
										, DXGI_FORMAT_R8G8B8A8_UNORM
										, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

			// DepthStencil Texture 를 생성한다.
			Ptr<CTexture> DSTex = CAssetMgr::GetInst()->CreateTexture(L"ShadowMapDSTex"
																	, 4096, 4096
																	, DXGI_FORMAT_D24_UNORM_S8_UINT
																	, D3D11_BIND_DEPTH_STENCIL);
			m_ShadowMapMRT->Create(&pShadowMap, 1, DSTex);
		}
	}

	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl");
		m_LightMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
		m_LightMtrl->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));
	}

	else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl");
	}
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}