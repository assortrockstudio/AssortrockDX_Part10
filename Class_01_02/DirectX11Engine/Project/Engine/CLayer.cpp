#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"


CLayer::CLayer()
{
}

CLayer::~CLayer()
{
	Safe_Del_Vector(m_vecParent);
}

void CLayer::begin()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->begin();
	}
}

void CLayer::tick()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->tick();
	}
}

void CLayer::finaltick()
{
	vector<CGameObject*>::iterator iter = m_vecParent.begin();

	for (; iter != m_vecParent.end(); )
	{
		(*iter)->finaltick();

		if ((*iter)->IsDead())
		{
			iter = m_vecParent.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLayer::AddObject(CGameObject* _Object, bool _ChildMove)
{
	// 최상위 부모 오브젝트
	if (!_Object->GetParent())
	{
		m_vecParent.push_back(_Object);		
	}	
	
	// 오브젝트가 자식을 보유하고 있다면.
	static list<CGameObject*> queue;
	queue.clear();
	queue.push_back(_Object);

	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		if (_ChildMove)
		{
			pObject->m_LayerIdx = m_LayerIdx;
		}
		else
		{
			// 자식까지 레이어 이동이 아니면, 레이어로 소속되는 부모 오브젝트만 레이어 인덱스 체크
			// 자식 오브젝트가 레이어 인덱스가 -1 이면(무소속), 이럴 경우는 예외적으로 레이어 인덱스 체크
			if (pObject == _Object || -1 == pObject->m_LayerIdx)
			{
				pObject->m_LayerIdx = m_LayerIdx;
			}		
		}

		vector<CGameObject*> vecChildren = pObject->GetChildren();
		for (size_t i = 0; i < vecChildren.size(); ++i)
		{
			queue.push_back(vecChildren[i]);
		}
	}
}



void CLayer::DeregisterParentObject(CGameObject* _Object)
{
	vector<CGameObject*>::iterator iter = m_vecParent.begin();

	for (iter; iter != m_vecParent.end(); ++iter)
	{
		if ((*iter) == _Object)
		{
			m_vecParent.erase(iter);
			return;
		}
	}

	assert(nullptr);
}