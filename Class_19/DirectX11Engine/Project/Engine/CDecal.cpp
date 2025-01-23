#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"


CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)	
	, m_AsLight(false)
	, m_LightPow(0.4f)
{
	SetDynamicShadow(false);

	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl"), 0);
	GetMaterial(0)->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
}

CDecal::~CDecal()
{
}

void CDecal::finaltick()
{
	DrawDebugCube(Transform()->GetWorldMat(), Vec4(0.f, 1.f, 0.f, 1.f), true, 0.f);
}

void CDecal::render()
{
	Transform()->Binding();

	if (nullptr != m_OutputTex)
		GetMaterial(0)->SetTexParam(TEX_1, m_OutputTex);

	GetMaterial(0)->SetScalarParam(INT_0, (int)m_AsLight);
	GetMaterial(0)->SetScalarParam(FLOAT_0, m_LightPow);
	GetMaterial(0)->Binding();

	GetMesh()->render(0);
}