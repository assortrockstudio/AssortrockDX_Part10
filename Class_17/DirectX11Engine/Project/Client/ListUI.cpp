#include "pch.h"
#include "ListUI.h"


ListUI::ListUI()
	: EditorUI("List", "##List")
    , m_SelectedIdx(-1)
{
}

ListUI::~ListUI()
{
}

void ListUI::render_tick()
{
    ImVec2 vWinSize = ImGui::GetWindowContentRegionMax();
    vWinSize.y -= 27;
    vWinSize.x -= 8;

    if (ImGui::BeginListBox("##ListBox", vWinSize))
    {
        for (size_t i = 0; i < m_vecList.size(); ++i)
        {
            bool bSelected = (i == m_SelectedIdx);                

            if (ImGui::Selectable(m_vecList[i].c_str(), bSelected))
            {
                m_SelectedIdx = i;
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if (m_CallBack)
                    m_CallBack();

                if (m_Inst && m_Delegate)
                    (m_Inst->*m_Delegate)((DWORD_PTR)&m_vecList[m_SelectedIdx]);

                SetActive(false);
            }
        }

        ImGui::EndListBox();
    }
}

void ListUI::Activate()
{
    m_vecList.insert(m_vecList.begin(), "None");
}

void ListUI::Deactivate()
{
    m_vecList.clear();
    m_SelectedIdx = -1;

    m_CallBack = nullptr;

    m_Inst = nullptr;
    m_Delegate = nullptr;

    SetModal(false);
}