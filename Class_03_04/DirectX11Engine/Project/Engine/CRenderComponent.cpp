#include "pch.h"
#include "CRenderComponent.h"


CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_FrustumCheck(true)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Other)
	: CComponent(_Other)
	, m_Mesh(_Other.m_Mesh)
	, m_CurMtrl(nullptr)
	, m_SharedMtrl(_Other.m_SharedMtrl)
	, m_DynamicMtrl(nullptr)
{
	// 원본 컴포넌트가 동적 재질을 보유하고 있고, 그걸 지금 사용 중이라면
	if (nullptr != _Other.m_DynamicMtrl && _Other.m_DynamicMtrl == _Other.m_CurMtrl)
	{
		// 복사되는 오브젝트도 자신만의 동적재질을 가져야 하고,
		GetDynamicMaterial();

		// 원본 오브젝트의 동적재질의 세팅값을 똑같이 받아와야 한다.
		m_DynamicMtrl = _Other.m_DynamicMtrl;
	}

	// 원본 컴포넌트의 현재 사용재질이 Shared 면, 복사본 컴포넌트도 똑같이 한다.
	else if (nullptr != _Other.m_SharedMtrl && _Other.m_SharedMtrl == _Other.m_CurMtrl)
	{
		m_CurMtrl = _Other.m_SharedMtrl;
	}
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	// 동적 재질을 세팅받을 수 없다.
	assert(!_Mtrl->IsDynamic());

	m_CurMtrl = m_SharedMtrl = _Mtrl;	
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// 레벨이 플레이모드에서만 사용 가능


	// 재질 자체를 참조한적이 없다면 nullptr 을 준다.
	if (nullptr == m_SharedMtrl)
		return nullptr;

	// 동적재질을 이전에 생성한 적이 있으면 그걸 준다.
	if (nullptr != m_DynamicMtrl)
	{
		m_CurMtrl = m_DynamicMtrl;
		return m_DynamicMtrl;
	}
	
	m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->GetDynamicMaterial();

	return m_DynamicMtrl;
}

void CRenderComponent::RestoreMaterial()
{
	m_CurMtrl = m_SharedMtrl;
	m_DynamicMtrl = nullptr;
}




void CRenderComponent::SaveToLevelFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_SharedMtrl, _File);
}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	LoadAssetRef(m_SharedMtrl, _File);

	SetMaterial(m_SharedMtrl);
}
