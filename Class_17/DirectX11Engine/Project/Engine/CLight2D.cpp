#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
	, m_LightIdx(-1)
{

}

CLight2D::~CLight2D()
{

}

void CLight2D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.LightType = (UINT)_Type;
}

void CLight2D::finaltick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	m_LightIdx = CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}
