#include "pch.h"
#include "CRenderComponent.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CTransform.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_FrustumCheck(true)
	, m_DynamicShadow(true)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Other)
	: CComponent(_Other)
	, m_Mesh(_Other.m_Mesh)	
	, m_FrustumCheck(_Other.m_FrustumCheck)
	, m_DynamicShadow(_Other.m_DynamicShadow)
{
	m_vecMtrls.resize(_Other.m_vecMtrls.size());

	for (size_t i = 0; i < _Other.m_vecMtrls.size(); ++i)
	{
		m_vecMtrls[i].pCurMtrl = _Other.m_vecMtrls[i].pCurMtrl;
		m_vecMtrls[i].pSharedMtrl = _Other.m_vecMtrls[i].pSharedMtrl;

		// 원본 오브젝트가 공유재질을 참조하고 있고, 현재 사용재질은 공유재질이 아닌경우
		if (_Other.m_vecMtrls[i].pSharedMtrl != _Other.m_vecMtrls[i].pCurMtrl)
		{
			assert(_Other.m_vecMtrls[i].pDynamicMtrl.Get());

			// 복사 렌더 컴포넌트도 별도의 동적재질을 생성한다.
			GetDynamicMaterial(i);

			// 원본 렌더컴포넌트의 동적재질 값을 현재 생성한 동적재질로 복사한다.
			*m_vecMtrls[i].pDynamicMtrl.Get() = *_Other.m_vecMtrls[i].pDynamicMtrl.Get();
		}
		else
		{
			m_vecMtrls[i].pCurMtrl = m_vecMtrls[i].pSharedMtrl;
		}
	}
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMesh(Ptr<CMesh> _Mesh)
{
	m_Mesh = _Mesh;

	if (!m_vecMtrls.empty())
	{
		m_vecMtrls.clear();
		vector<tMtrlSet> vecMtrls;
		m_vecMtrls.swap(vecMtrls);
	}

	if (nullptr != m_Mesh)
		m_vecMtrls.resize(m_Mesh->GetSubsetCount());
}


void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx)
{
	// 동적 재질을 세팅받을 수 없다.
	assert(!_Mtrl->IsDynamic());

	// 재질이 변경되면 기존에 복사본 받아둔 DynamicMaterial 을 삭제한다.
	m_vecMtrls[_idx].pSharedMtrl = _Mtrl;
	m_vecMtrls[_idx].pCurMtrl = _Mtrl;
	m_vecMtrls[_idx].pDynamicMtrl = nullptr;
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _idx)
{
	return m_vecMtrls[_idx].pCurMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _idx)
{
	// 공유재질을 가져오는것으로 현재 사용재질을 동적재질에서 회복하도록 한다
	m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;

	if (m_vecMtrls[_idx].pDynamicMtrl.Get())
	{
		m_vecMtrls[_idx].pDynamicMtrl = nullptr;
	}

	return m_vecMtrls[_idx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _idx)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pCurLevel->GetState() != LEVEL_STATE::PLAY)
		return nullptr;

	// 원본 재질이 없다 -> Nullptr 반환
	if (nullptr == m_vecMtrls[_idx].pSharedMtrl)
	{
		m_vecMtrls[_idx].pCurMtrl = nullptr;
		m_vecMtrls[_idx].pDynamicMtrl = nullptr;
		return m_vecMtrls[_idx].pCurMtrl;
	}

	if (nullptr == m_vecMtrls[_idx].pDynamicMtrl)
	{
		m_vecMtrls[_idx].pDynamicMtrl = m_vecMtrls[_idx].pSharedMtrl->Clone();
		m_vecMtrls[_idx].pDynamicMtrl->SetName(m_vecMtrls[_idx].pSharedMtrl->GetName() + L"_Clone");
		m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pDynamicMtrl;
	}

	return m_vecMtrls[_idx].pCurMtrl;
}

void CRenderComponent::render(UINT _iSubset)
{
	render();
}

void CRenderComponent::render_shadowmap()
{
	Transform()->Binding();

	Ptr<CMaterial> pShadowMapMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShadowMapMtrl");
	pShadowMapMtrl->Binding();
	
	GetMesh()->render(0);
}


ULONG64 CRenderComponent::GetInstID(UINT _iMtrlIdx)
{
	if (m_Mesh == nullptr || m_vecMtrls[_iMtrlIdx].pCurMtrl == nullptr)
		return 0;

	uInstID id{ (UINT)m_Mesh->GetID(), (WORD)m_vecMtrls[_iMtrlIdx].pCurMtrl->GetID(), (WORD)_iMtrlIdx };
	return id.llID;
}

void CRenderComponent::SaveToLevelFile(FILE* _File)
{
	// 메쉬 참조정보 저장
	SaveAssetRef(m_Mesh, _File);

	// 재질 참조정보 저장
	UINT iMtrlCount = GetMtrlCount();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _File);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		SaveAssetRef(m_vecMtrls[i].pSharedMtrl, _File);
	}

	fwrite(&m_DynamicShadow, 1, 1, _File);
}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
	// 메쉬 참조정보 불러오기
	LoadAssetRef(m_Mesh, _File);

	// 재질 참조정보 불러오기
	UINT iMtrlCount = GetMtrlCount();
	fread(&iMtrlCount, sizeof(UINT), 1, _File);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		LoadAssetRef(m_vecMtrls[i].pSharedMtrl, _File);
	}

	fread(&m_DynamicShadow, 1, 1, _File);
}
