#include "pch.h"
#include "TreeUI.h"

// =========
// Tree Node
// =========
UINT TreeNode::g_ID = 0;

TreeNode::~TreeNode()
{
	Safe_Del_Vector(m_vecChildNode);
}

void TreeNode::render_tick()
{
	// TreeNode 의 출력 이름 설정
	string name;
	if (m_Name.empty())
		name = string("None") + "##" + m_ID;
	else
		name = m_Name + "##" + m_ID;
	
		
	// 화살표를 누르거나, 더블클릭한 경우에 자식노드를 펼친다.
	UINT iFlag = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

	// 프레임 설정
	if (m_bFrame)
		iFlag |= ImGuiTreeNodeFlags_Framed;

	if (m_bSelected)
		iFlag |= ImGuiTreeNodeFlags_Selected;

	

	// 파일 이름 설정
	if (m_Owner->m_bShowFileName)
	{
		char buffer[256] = {};
		_splitpath_s(name.c_str(), 0, 0, 0, 0, buffer, 256, 0, 0);
		name = buffer;
	}

	// 화살표 설정
	if (m_vecChildNode.empty())
	{
		iFlag |= ImGuiTreeNodeFlags_Leaf;
		if (m_bFrame)
			name = "   " + name;
	}

	if (ImGui::TreeNodeEx(name.c_str(), iFlag))
	{
		// 노드의 소유자(TreeUI) 가 Drag 기능을 사용하기로 했다면...
		if (m_Owner->m_bUseDrag)
		{
			if (ImGui::BeginDragDropSource())
			{
				// TreeUI 에 본인이 Drag 노드임을 등록
				m_Owner->SetDragNode(this);

				// 노드의 소유자(TreeUI), 의 Parent 는 TreeUI 를 자식으로 두고있는 부모UI(ex Outliner, ContentUI)				
				ImGui::SetDragDropPayload(m_Owner->GetParentUI()->GetName().c_str(), &m_Data, sizeof(DWORD_PTR));

				// PayLoad 후, EndDratDrop 전에 사용된 Text 는 툴팁으로 적용된다.
				ImGui::Text(m_Name.c_str());
				
				ImGui::EndDragDropSource();
			}
		}

		if (m_Owner->m_bUseDrag && m_Owner->m_bUseDrop)
		{
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_Owner->GetParentUI()->GetName().c_str());

				if (nullptr != payload)
				{
					m_Owner->SetDropNode(this);
				}

				ImGui::EndDragDropTarget();
			}
		}



		// 해당 노드위에서 마우스 해제가 발생하면 해당 노드를 선택된 상태로 만든다.
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{	
			if (ImGui::IsItemHovered())
			{
				// 프레임 노드는 선택개념이 없다.
				if (!m_bFrame)
				{
					m_Owner->SetSelectedNode(this);
				}
			}			
		}

		// 자식 노드 render
		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_tick();
		}

		ImGui::TreePop();
	}
}













// ========
// TreeUI
// ========
TreeUI::TreeUI(const string& _Name)
	: EditorUI(_Name, "##Tree")
	, m_RootNode(nullptr)
	, m_bShowRoot(true)
	, m_bShowFileName(false)
	, m_SelectedNode(nullptr)
	, m_DragNode(nullptr)
	, m_DropNode(nullptr)
	, m_bUseDrag(false)
	, m_bUseDrop(false)
	, m_SelectedCallBack(nullptr)
	, m_SelectedInst(nullptr)
	, m_SelectedDelegate(nullptr)
	, m_DragDropInst(nullptr)
	, m_DragDropDelegate(nullptr)
{
}

TreeUI::~TreeUI()
{
	if (nullptr != m_RootNode)
	{
		delete m_RootNode;
	}
}

TreeNode* TreeUI::AddTreeNode(TreeNode* _Parent, const string& _NodeName, DWORD_PTR _dwData)
{
	TreeNode* pNewNode = new TreeNode(_NodeName, _dwData);

	// 노드의 소유자가 트리임을 세팅함
	pNewNode->m_Owner = this;

	// 부모노드를 지정하지 않으면, 루트노드로 추가하겠다는 의미	
	if (nullptr == _Parent)
	{
		// 이미 루트가 TreeUI 에 존재하면 assert
		assert(!m_RootNode);

		m_RootNode = pNewNode;		
	}

	else
	{
		_Parent->AddChildNode(pNewNode);
	}


	return pNewNode;
}


void TreeUI::render_tick()
{	
	// 트리에 노드가 하나도 없는 경우
	if (nullptr == m_RootNode)
		return;

	if (m_bShowRoot)
	{
		m_RootNode->render_tick();
	}

	else
	{
		const vector<TreeNode*>& vecChildNode = m_RootNode->GetChildNode();

		for (size_t i = 0; i < vecChildNode.size(); ++i)
		{
			vecChildNode[i]->render_tick();
		}
	}

	if (m_DragNode && (m_DropNode || ImGui::IsMouseReleased(ImGuiMouseButton_Left)))
	{
		if (m_DragDropInst && m_DragDropDelegate)
		{
			(m_DragDropInst->*m_DragDropDelegate)((DWORD_PTR)m_DragNode, (DWORD_PTR)m_DropNode);
		}

		m_DragNode = nullptr;
		m_DropNode = nullptr;
	}
}


void TreeUI::SetSelectedNode(TreeNode* _Node)
{
	// 이전에 선택된 노드가 있으면 선택상태를 해제
	if (m_SelectedNode)
		m_SelectedNode->m_bSelected = false;

	m_SelectedNode = _Node;

	if (m_SelectedNode)
	{
		m_SelectedNode->m_bSelected = true;

		// 등록되어있는 CallBack, Delegate 가 있으면 호출시켜준다.
		if (m_SelectedCallBack)
		{
			m_SelectedCallBack();
		}

		if (m_SelectedInst && m_SelectedDelegate)
		{
			(m_SelectedInst->*m_SelectedDelegate)((DWORD_PTR)m_SelectedNode);
		}
	}	
}