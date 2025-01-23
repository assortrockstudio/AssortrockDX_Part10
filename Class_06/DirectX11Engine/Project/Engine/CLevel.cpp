#include "pch.h"
#include "CLevel.h"

#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderMgr.h"

CLevel::CLevel()
	: m_arrLayer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_LayerIdx = i;
	}
}

CLevel::~CLevel()
{
	Safe_Del_Array(m_arrLayer);
}



void CLevel::begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->begin();
	}
}

void CLevel::tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->tick();
	}
}

void CLevel::finaltick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->finaltick();
	}
}

void CLevel::RegisterClear()
{
	// 이전 프레임에 등록된 오브젝트들을 제거
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i]->Clear();
	}

}

void CLevel::AddObject(UINT _LayerIdx, CGameObject* _Object, bool _bChildMove)
{
	m_arrLayer[_LayerIdx]->AddObject(_Object, _bChildMove);
}

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
	if (LEVEL_STATE::STOP == _NextState || LEVEL_STATE::PAUSE == _NextState)
		CRenderMgr::GetInst()->ChangeRenderMode(RENDER_MODE::EDITOR);

	// 동일한 상태로의 전환이 발생한 경우, return
	if (m_State == _NextState)
		return;

	// 정지상태에서 일시정지로 넘어가는 경우
	assert(!(LEVEL_STATE::STOP == m_State && LEVEL_STATE::PAUSE == _NextState));

	// Stop -> Play 로 전환될때 레벨에 Begin 을 호출한다.
	if (LEVEL_STATE::STOP == m_State && LEVEL_STATE::PLAY == _NextState)
	{
		begin();
	}

	// Stop or Pause --> Play 
	// RenderMode 를 Play 모드로 변경한다.
	if ((LEVEL_STATE::STOP == m_State || LEVEL_STATE::PAUSE == m_State) && LEVEL_STATE::PLAY == _NextState)
	{
		CRenderMgr::GetInst()->ChangeRenderMode(RENDER_MODE::PLAY);
	}

	m_State = _NextState;
}


CGameObject* CLevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i]->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_Name == vecObjects[j]->GetName())
			{
				return vecObjects[j];
			}
		}
	}	

	return nullptr;
}

void CLevel::FindObjectByName(const wstring& _Name, vector<CGameObject*>& _vecObj)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i]->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_Name == vecObjects[j]->GetName())
			{
				_vecObj.push_back(vecObjects[j]);
			}
		}
	}
}

CGameObject* CLevel::FindObjectByComponent(COMPONENT_TYPE _Type, const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i]->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (vecObjects[j]->GetComponent(_Type))
			{
				return vecObjects[j];
			}
		}
	}
}

void CLevel::FindObjectByComponent(COMPONENT_TYPE _Type, vector<CGameObject*>& _vecObj)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i]->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (vecObjects[j]->GetComponent(_Type))
			{
				_vecObj.push_back(vecObjects[j]);
			}
		}
	}
}