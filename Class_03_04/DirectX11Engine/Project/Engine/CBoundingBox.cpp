#include "pch.h"
#include "CBoundingBox.h"

#include "CRenderMgr.h"

#include "CGameObject.h"
#include "CTransform.h"


CBoundingBox::CBoundingBox()
	: CComponent(COMPONENT_TYPE::BOUNDINGBOX)
	, m_Radius(0.f)
	, m_Absolute(false)
{
}

CBoundingBox::~CBoundingBox()
{
}

void CBoundingBox::finaltick()
{
	// Local Scale
	Matrix matScale = XMMatrixScaling(m_Radius, m_Radius, m_Radius);
	
	// Local Translation
	Matrix matTranslation = XMMatrixTranslation(m_OffsetPos.x, m_OffsetPos.y, m_OffsetPos.z);

	Vec3 ObjectPos = Transform()->GetWorldPos();
	Matrix matObjTrans = XMMatrixTranslation(ObjectPos.x, ObjectPos.y, ObjectPos.z);


	Vec3 ObjectScale = Transform()->GetWorldScale();
	float MaxScale = ObjectScale.x;
	if (MaxScale < ObjectScale.y)
		MaxScale = ObjectScale.y;
	if (MaxScale < ObjectScale.z)
		MaxScale = ObjectScale.z;
	Matrix matObjScale = XMMatrixScaling(MaxScale, MaxScale, MaxScale);

	if(m_Absolute)
		m_WorldMat = matScale * matTranslation * matObjTrans;
	else
		m_WorldMat = matScale * matTranslation * matObjScale * matObjTrans;


	if (CRenderMgr::GetInst()->IsShowBoundingBox())
	{
		DrawDebugSphere(m_WorldMat.Translation(), m_WorldMat._11, Vec4(0.f, 0.f, 1.f, 1.f), true, 0.f);
	}	
}

void CBoundingBox::SaveToLevelFile(FILE* _File)
{

}

void CBoundingBox::LoadFromLevelFile(FILE* _File)
{

}