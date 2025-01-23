#include "pch.h"
#include "CGameObject.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CComponent.h"
#include "CRenderComponent.h"
#include "CScript.h"

#include "CTaskMgr.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)
{
}

CGameObject::CGameObject(const CGameObject& _Other)
	: CEntity(_Other)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false) // 오브젝트 Life 체크	
{
	// 원본 오브젝트의 컴포넌트들을 복제해서 받아옴
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _Other.m_arrCom[i])
		{
			AddComponent(_Other.m_arrCom[i]->Clone());			
		}
	}
		
	for (size_t i = 0; i < _Other.m_vecScripts.size(); ++i)
	{
		AddComponent(_Other.m_vecScripts[i]->Clone());
	}
	
	for (size_t i = 0; i < _Other.m_vecChild.size(); ++i)
	{
		AddChild(_Other.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
	Safe_Del_Vector(m_vecScripts);
	Safe_Del_Vector(m_vecChild);
}

void CGameObject::begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->begin();
	}


	// 자식 오브젝트 begin 호출
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->tick();
	}

	// 자식 오브젝트 tick 호출
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	// Component finaltick 호출
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	// 소속 Layer에 자신을 등록
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(m_LayerIdx); 
	pLayer->RegisterObject(this);


	// 자식 오브젝트 finaltick 호출
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}
}

void CGameObject::render()
{
	if (m_RenderCom)
	{
		m_RenderCom->render();
	}
}

void CGameObject::AddComponent(CComponent* _Component)
{
	// 입력으로 들어오는 컴포넌트의 타입을 확인한다.
	COMPONENT_TYPE type = _Component->GetComponentType();

	// 입력된 컴포넌트가 Script 종류인 경우
	if (COMPONENT_TYPE::SCRIPT == type)
	{
		m_vecScripts.push_back((CScript*)_Component);
	}

	else
	{
		// 입력으로 들어온 컴포넌트를 이미 가지고 있는 경우
		assert(!m_arrCom[(UINT)type]);

		// 입력된 컴포넌트가 렌더링 관련 컴포넌트인지 확인
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);
		if (nullptr != pRenderCom)
		{
			// 이미 렌더링 관련 컴포넌트를 보유한 경우
			assert(!m_RenderCom);

			m_RenderCom = pRenderCom;
		}

		// 입력된 컴포넌트를 배열의 알맞은 인덱스 자리에 주소값을 기록한다.
		m_arrCom[(UINT)type] = _Component;
	}

	// 컴포넌트의 소유 오브젝트가 본인임을 알림
	_Component->m_Owner = this;
}


void CGameObject::DisconnectWidthParent()
{
	if (nullptr == m_Parent)
		return;
	
	vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (this == (*iter))
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	// 자식 오브젝트는 부모를 지정했는데, 부모 오브젝트가 자식을 모르는 경우
	assert(nullptr);
}

void CGameObject::RegisterAsParentObject()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pCurLayer = pCurLevel->GetLayer(m_LayerIdx);
	pCurLayer->AddObject(this, false);
}

void CGameObject::AddChild(CGameObject* _ChildObj)
{
	// 입력으로 들어오는 오브젝트가 이미 다른 부모의 자식인 경우
	if (_ChildObj->m_Parent)
	{
		_ChildObj->DisconnectWidthParent();
	}

	// 입력으로 들어오는 오브젝트가 최상위 부모 오브젝트, 레벨 내에 소속되어있는 경우
	else if(-1 != _ChildObj->m_LayerIdx)
	{
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CLayer* pCurLayer = pCurLevel->GetLayer(_ChildObj->m_LayerIdx);
		pCurLayer->DeregisterParentObject(_ChildObj);
	}
	
	_ChildObj->m_Parent = this;
	m_vecChild.push_back(_ChildObj);
}

bool CGameObject::IsAncestor(CGameObject* _Object)
{
	CGameObject* pAncestor = m_Parent;

	while (pAncestor)
	{
		if (pAncestor == _Object)
			return true;

		pAncestor = pAncestor->GetParent();
	}

	return false;
}

void CGameObject::Destroy()
{
	tTask task = {};

	task.Type = TASK_TYPE::DESTROY_OBJECT;
	task.dwParam_0 = (DWORD_PTR)this;

	CTaskMgr::GetInst()->AddTask(task);
}