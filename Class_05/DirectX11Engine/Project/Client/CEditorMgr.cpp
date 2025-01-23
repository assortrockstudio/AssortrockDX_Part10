#include "pch.h"
#include "CEditorMgr.h"

#include <Engine/CRenderMgr.h>

#include "CGameObjectEx.h"
#include <Engine/components.h>

#include "CEditorCameraScript.h"


CEditorMgr::CEditorMgr()
{

}

CEditorMgr::~CEditorMgr()
{
	Safe_Del_Vector(m_vecEditorObj);
}

void CEditorMgr::init()
{
	CGameObject* pEditorCam = new CGameObjectEx;
	pEditorCam->AddComponent(new CTransform);
	pEditorCam->AddComponent(new CCamera);
	pEditorCam->AddComponent(new CEditorCameraScript);

	pEditorCam->Camera()->LayerCheckAll();
	pEditorCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pEditorCam->Camera()->SetFar(100000.f);

	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCam->Camera());

	m_vecEditorObj.push_back(pEditorCam);
}

void CEditorMgr::tick()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}
