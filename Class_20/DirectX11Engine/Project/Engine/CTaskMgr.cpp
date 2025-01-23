#include "pch.h"
#include "CTaskMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CCollider2D.h"

#include "CRenderMgr.h"
#include "CAssetMgr.h"

CTaskMgr::CTaskMgr()
	: m_ObjectEvent(false)
{

}

CTaskMgr::~CTaskMgr()
{

}

void CTaskMgr::tick()
{
	ClearDeadObject();

	ClearEvent();

	for (size_t i = 0; i < m_vecTasks.size(); ++i)
	{
		ExecuteTask(m_vecTasks[i]);
	}

	m_vecTasks.clear();
}

void CTaskMgr::ClearDeadObject()
{
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		delete m_vecDead[i];
	}

	m_vecDead.clear();
}

void CTaskMgr::ExecuteTask(tTask& _Task)
{
	switch (_Task.Type)
	{
	case TASK_TYPE::SPAWN_OBJECT:
		// Param 0 : Layer Index
		// Param 1 : Object Adress
	{
		int LayerIdx = (int)_Task.dwParam_0;
		CGameObject* pSpawnObj = (CGameObject*)_Task.dwParam_1;
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		pCurLevel->AddObject(LayerIdx, pSpawnObj);

		// 레벨에 합류하는 오브젝트는 Begin 을 호출받는다.
		if (LEVEL_STATE::PLAY == pCurLevel->GetState())
		{
			pSpawnObj->begin();
		}		

		m_ObjectEvent = true;
	}
		break;

	case TASK_TYPE::DESTROY_OBJECT:
		// Param 0 : Object Adress
	{
		CGameObject* pObject = (CGameObject*)_Task.dwParam_0;

		if (!pObject->IsDead())
		{
			pObject->m_Dead = true;
			m_vecDead.push_back(pObject);
		}

		m_ObjectEvent = true;
	}
		break;

	case TASK_TYPE::COLLIDER2D_SEMI_DEACTIVE:
	{
		CCollider2D* pCollider = (CCollider2D*)_Task.dwParam_0;		
		pCollider->m_SemiDeactive = true;
	}		
		break;

	case TASK_TYPE::COLLIDER2D_DEACTIVE:
	{
		CCollider2D* pCollider = (CCollider2D*)_Task.dwParam_0;
		pCollider->m_SemiDeactive = false;
		pCollider->m_Active = false;
	}
		break;
	case TASK_TYPE::CHANGE_LEVEL:
	{
		CLevel* pNextLevel = (CLevel*)_Task.dwParam_0;
		LEVEL_STATE NextLevelState = (LEVEL_STATE)_Task.dwParam_1;

		CRenderMgr::GetInst()->ClearRegisteredCamera();

		CLevelMgr::GetInst()->ChangeLevel(pNextLevel);
		pNextLevel->ChangeState(NextLevelState);

		m_ObjectEvent = true;
	}
		break;

	case TASK_TYPE::CHANGE_LEVEL_STATE:
	{
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		LEVEL_STATE NextLevelState = (LEVEL_STATE)_Task.dwParam_0;

		pCurLevel->ChangeState(NextLevelState);		
	}
	break;

	case TASK_TYPE::DEL_ASSET:
	{
		CAsset* pAsset = (CAsset*)_Task.dwParam_0;
		int RefCount = pAsset->GetRefCount();

		if (1 < RefCount)
		{
			int id = MessageBox(nullptr, L"해당 Asset 이 다른곳에서 참조되고 있습니다.\n 계속 진행하시겠습니까?"
				     , L"Asset 삭제 경고", MB_YESNO);
			if (IDCANCEL == id)
				break;			
		}

		CAssetMgr::GetInst()->DeleteAsset(pAsset->GetAssetType(), pAsset->GetKey());
	}
		break;
	}	
}
