#include "pch.h"
#include "OutlinerUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTaskMgr.h>


#include "Inspector.h"
#include "TreeUI.h"

OutlinerUI::OutlinerUI()
	: EditorUI("Outliner", "##Outliner")
{
	// 자식 UI 로 TreeUI 생성
	m_Tree = new TreeUI("OutlinerTree");	
	AddChildUI(m_Tree);

	// TreeUI 의 루트노드를 보이지않게 설정
	m_Tree->ShowRoot(false);

	// 항목 선택시 동작할 CallBack OR Delegate 등록	
	m_Tree->RegisterSelectDelegate(this, (UI_DELEGATE1)&OutlinerUI::SelectGameObject);

	// 드래그 드랍 기능 On, 
	m_Tree->UseDragDrop(true);	

	// 드래그 드랍 발생시 동작할 Delegate 등록
	m_Tree->RegisterDragDropDelegate(this, (UI_DELEGATE2)&OutlinerUI::DragDropFunc);
	
	// 현재 Asset 의 상태를 트리에 반영
	RenewGameObject();
}

OutlinerUI::~OutlinerUI()
{
}

void OutlinerUI::render_tick()
{
	if (CTaskMgr::GetInst()->IsObjectChanged())
	{
		RenewGameObject();
	}
}

void OutlinerUI::RenewGameObject()
{
	// Tree 를 Clear 한다.
	m_Tree->Clear();
	TreeNode* pRootNode = m_Tree->AddTreeNode(nullptr, "DummyRoot");

	// 현재 레벨을 알아낸다.
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (nullptr == pCurLevel)
		return;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);

		const vector<CGameObject*>& vecParent = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecParent.size(); ++i)
		{
			AddGameObject(pRootNode, vecParent[i]);
		}
	}
}


void OutlinerUI::AddGameObject(TreeNode* _Node, CGameObject* _Object)
{
	TreeNode* pNewNode = m_Tree->AddTreeNode(_Node, ToString(_Object->GetName()), (DWORD_PTR)_Object);
	
	// 만약 해당 오브젝트가 자식 오브젝트를 보유하고 있다면
	const vector<CGameObject*>& vecChildren = _Object->GetChildren();

	// 자식 오브젝틀 부모오브젝트의 정보로 만들어진 노드 밑으로 다시 추가한다(재귀)
	for (size_t i = 0; i < vecChildren.size(); ++i)
	{
		AddGameObject(pNewNode, vecChildren[i]);
	}
}

UINT OutlinerUI::SelectGameObject(DWORD_PTR _Data)
{
	TreeNode* pSelectedNode = (TreeNode*)_Data;

	CGameObject* pSelectedObject = (CGameObject*)pSelectedNode->GetData();

	Inspector* pInspector = CImGuiMgr::GetInst()->FindEditorUI<Inspector>("Inspector");

	pInspector->SetTargetObject(pSelectedObject);

	return 0;
}

// 트리에서 Drag Drop 이 발생하면 호출되는 함수
UINT OutlinerUI::DragDropFunc(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
{
	TreeNode* pDragNode = (TreeNode*)_DragNode;
	TreeNode* pDropNode = (TreeNode*)_DropNode;
		
	CGameObject* pDragObj = (CGameObject*)pDragNode->GetData();
	CGameObject* pDropObj = pDropNode ? (CGameObject*)pDropNode->GetData() : nullptr;
	
	// 드랍 오브젝트가 존재하면
	if (pDropObj)
	{
		// 드래그 된 오브젝트가 드랍될 오브젝트의 조상이면 예외처리
		if (false == pDropObj->IsAncestor(pDragObj))
		{
			// 부모자식 관계로 연결
			pDropObj->AddChild(pDragObj);
		}
	}

	// 드랍된 곳이 허공이면
	else
	{
		if (nullptr != pDragObj->GetParent())
		{
			// 부모로부터 나와서, 최상위 오브젝트가 되어야 한다.
			pDragObj->DisconnectWidthParent();
			pDragObj->RegisterAsParentObject();
		}		
	}
	

	// 오브젝트의 계층 구조(부모 자식) 가 변경되었기 때문에
	// Outliner 의 트리를 재구성한다.
	RenewGameObject();

	return 0;
}
