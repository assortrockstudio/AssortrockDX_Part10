#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_Info{}
	, m_LightIdx(-1)
{
}

CLight3D::~CLight3D()
{
}

void CLight3D::finaltick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	m_LightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);

	// DebugRender ¿äÃ»
	if (m_Info.LightType == (UINT)LIGHT_TYPE::POINT)
		DrawDebugSphere(m_Info.WorldPos, m_Info.Range, Vec4(1.f, 1.f, 0.f, 1.f), false, 0.f);
	else
		DrawDebugCube(m_Info.WorldPos, Vec3(50.f, 50.f, 200.f), Transform()->GetRelativeRotation(), Vec4(1.f, 1.f, 0.f, 1.f), true, 0.f);
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