#include "pch.h"
#include "CCollider2D.h"

#include "CTransform.h"
#include "CScript.h"

#include "CTaskMgr.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_Scale(Vec3(1.f, 1.f, 1.f))
	, m_Absolute(false)
	, m_Active(true)
	, m_SemiDeactive(false)
{

}

CCollider2D::CCollider2D(const CCollider2D& _Other)
	: CComponent(_Other)
	, m_Offset(_Other.m_Offset)
	, m_FinalPos(_Other.m_FinalPos)
	, m_Scale(_Other.m_Scale)
	, m_Rotation(_Other.m_Rotation)
	, m_Absolute(_Other.m_Absolute)
	, m_OverlapCount(0)
	, m_Active(_Other.m_Active)
	, m_SemiDeactive(false)
{
}

CCollider2D::~CCollider2D()
{

}

void CCollider2D::Activate()
{
	m_Active = true;	
}


void CCollider2D::finaltick()
{
	// 비활성화 예정 상태면, 비활성화 시킨다.
	if (m_SemiDeactive)
	{
		CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::COLLIDER2D_DEACTIVE, (DWORD_PTR)this, });
	}

	// 비활성화된 충돌체는 계산하지 않는다.
	else if (!m_Active)
		return;
	

	// 최종 위치값을 계산
	Vec3 vObjPos = Transform()->GetRelativePos();
	m_FinalPos = vObjPos + m_Offset;

	if (false == m_Absolute)
	{
		Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		Matrix matRot = XMMatrixRotationZ(m_Rotation.z);
		Matrix matTrans = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);

		m_matColWorld = matScale * matRot * matTrans;
		m_matColWorld *= Transform()->GetWorldMat();
	}
	else
	{
		Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		Matrix matRot = XMMatrixRotationZ(m_Rotation.z);
		Matrix matTrans = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);

		m_matColWorld = matScale * matRot * matTrans;

		Matrix matObjScaleInv = XMMatrixIdentity();
		Vec3 vObjScale = Transform()->GetRelativeScale();
		matObjScaleInv = XMMatrixScaling(vObjScale.x, vObjScale.y, vObjScale.z);
		matObjScaleInv = XMMatrixInverse(nullptr, matObjScaleInv);

		m_matColWorld = m_matColWorld * matObjScaleInv * Transform()->GetWorldMat();
	}

	if (0 == m_OverlapCount)
		DrawDebugRect(m_matColWorld, Vec4(0.f, 1.f, 0.f, 1.f), false, 0.f);
	else if (1 <= m_OverlapCount)
		DrawDebugRect(m_matColWorld, Vec4(1.f, 0.2f, 0.2f, 1.f), false, 0.f);
	else
		assert(nullptr);
}

void CCollider2D::BeginOverlap(CCollider2D* _OtherCollider)
{
	++m_OverlapCount;
	
	const vector<CScript*>& vecScripts = GetOwner()->GetScripts();

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->BeginOverlap(this, _OtherCollider->GetOwner(), _OtherCollider);
	}	
}

void CCollider2D::Overlap(CCollider2D* _OtherCollider)
{
	const vector<CScript*>& vecScripts = GetOwner()->GetScripts();

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->Overlap(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

void CCollider2D::EndOverlap(CCollider2D* _OtherCollider)
{
	--m_OverlapCount;

	const vector<CScript*>& vecScripts = GetOwner()->GetScripts();

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->EndOverlap(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

void CCollider2D::Deactivate()
{
	tTask task = {};
	task.Type = TASK_TYPE::COLLIDER2D_SEMI_DEACTIVE;
	task.dwParam_0 = (DWORD_PTR)this;
	CTaskMgr::GetInst()->AddTask(task);
}

void CCollider2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Offset, sizeof(Vec3), 1, _File);
	fwrite(&m_Scale, sizeof(Vec3), 1, _File);
	fwrite(&m_Rotation, sizeof(Vec3), 1, _File);
	fwrite(&m_Absolute, sizeof(bool), 1, _File);
	fwrite(&m_Active, sizeof(bool), 1, _File);
	fwrite(&m_SemiDeactive, sizeof(bool), 1, _File);
}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Offset, sizeof(Vec3), 1, _File);
	fread(&m_Scale, sizeof(Vec3), 1, _File);
	fread(&m_Rotation, sizeof(Vec3), 1, _File);
	fread(&m_Absolute, sizeof(bool), 1, _File);
	fread(&m_Active, sizeof(bool), 1, _File);
	fread(&m_SemiDeactive, sizeof(bool), 1, _File);
}