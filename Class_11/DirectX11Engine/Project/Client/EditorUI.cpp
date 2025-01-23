#include "pch.h"
#include "EditorUI.h"

EditorUI::EditorUI(const string& _Name, const string& _ID)
	: m_Name(_Name)
	, m_ID(_ID)
	, m_ParentUI(nullptr)
	, m_Active(true)
	, m_Seperate(false)
	, m_Modal(false)
	, m_Menu(false)
	, m_Move(true)
{
}

EditorUI::~EditorUI()
{
	Safe_Del_Vector(m_vecChildUI);
}

bool EditorUI::SetActive(bool _Active)
{
	if (m_Active == _Active)
		return false;

	m_Active = _Active;

	if (m_Active)
		Activate();
	else
		Deactivate();

	return true;
}

void EditorUI::SetFocus()
{
	string fullname = m_Name + m_ID;

	ImGui::SetWindowFocus(fullname.c_str());
}

void EditorUI::tick()
{
	// 비활성화 상태면 tick 함수를 실행하지 않음
	if (!m_Active)
		return;

	string fullname = m_Name + m_ID;

	UINT Flag = 0;
	if (m_Menu)
		Flag |= ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar;
	if(!m_Move)
		Flag |= ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;

	if (IsRootUI())
	{		
		bool bActive = m_Active;

		// Modaless
		if (!m_Modal)
		{	
			ImGui::Begin(fullname.c_str(), &bActive, Flag);

			SetActive(bActive);

			render_tick();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->tick();
			}

			ImGui::End();
		}

		// Modal
		else
		{
			ImGui::OpenPopup(fullname.c_str());
			if (ImGui::BeginPopupModal(fullname.c_str(), &bActive, Flag))
			{
				SetActive(bActive);

				render_tick();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->tick();
				}

				ImGui::EndPopup();
			}
			else
			{
				SetActive(bActive);
			}			
		}	
	}

	else
	{
		if (m_Seperate)
			ImGui::Separator();

		ImGui::BeginChild(fullname.c_str(), m_vChildSize);

		render_tick();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->tick();
		}

		ImGui::EndChild();
	}
}