#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"


CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_Absolute(false)
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	m_matWorld = XMMatrixIdentity();

	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);
	
	Matrix matRot = XMMatrixRotationX(m_RelativeRotation.x);
	matRot *= XMMatrixRotationY(m_RelativeRotation.y);
	matRot *= XMMatrixRotationZ(m_RelativeRotation.z);
	
	Matrix matTranslation = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	m_matWorld = matScale * matRot * matTranslation;

	// 오브젝트의 방향정보 계산
	m_WorldDir[(UINT)DIR_TYPE::RIGHT]	= m_RelativeDir[(UINT)DIR_TYPE::RIGHT]	= XAxis;
	m_WorldDir[(UINT)DIR_TYPE::UP]		= m_RelativeDir[(UINT)DIR_TYPE::UP]		= YAxis;
	m_WorldDir[(UINT)DIR_TYPE::FRONT]	= m_RelativeDir[(UINT)DIR_TYPE::FRONT]	= ZAxis;

	for (int i = 0; i < 3; ++i)
	{		
		m_RelativeDir[i] = XMVector3TransformNormal(m_RelativeDir[i], matRot);
		m_RelativeDir[i].Normalize();
	}

	// 부모 오브젝트가 있는 경우
	if (GetOwner()->GetParent())
	{
		const Matrix& matParentWorldMat = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_Absolute)
		{
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
			Matrix matScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z));
			m_matWorld = m_matWorld * matScaleInv * matParentWorldMat;
		}
		else
		{
			m_matWorld *= matParentWorldMat;
		}
		

		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = XMVector3TransformNormal(m_WorldDir[i], m_matWorld);
			m_WorldDir[i].Normalize();
		}
	}

	// 부모 오브젝트가 없는경우(본인이 최상위 부모 오브젝트)
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = m_RelativeDir[i];
		}
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::Binding()
{
	// SysMem -> GPU
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
		
	g_Trans.matWorld = m_matWorld;
	g_Trans.matWorldInv = m_matWorldInv;

	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;

	pCB->SetData(&g_Trans);
	pCB->Binding();
}

Vec3 CTransform::GetWorldPos()
{	
	return m_matWorld.Translation();
}

Vec3 CTransform::GetWorldScale()
{
	CGameObject* pObject = GetOwner();
	Vec3 vWorldScale = Vec3(1.f, 1.f, 1.f);

	while (pObject)
	{
		vWorldScale *= pObject->Transform()->GetRelativeScale();

		if (pObject->Transform()->IsAbsolute())
			break;

		pObject = pObject->GetParent();
	}

	return vWorldScale;
}

Vec3 CTransform::GetWorldRotation()
{
	return Vec3();
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_Absolute, sizeof(bool), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _File)
{
	fread(&m_RelativePos, sizeof(Vec3), 1, _File);
	fread(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_RelativeRotation, sizeof(Vec3), 1, _File);
	fread(&m_Absolute, sizeof(bool), 1, _File);
}
