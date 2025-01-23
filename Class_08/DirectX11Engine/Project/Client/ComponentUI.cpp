#include "pch.h"
#include "ComponentUI.h"

ComponentUI::ComponentUI(const string& _Name, const string& _ID, COMPONENT_TYPE _Type)
	: EditorUI(_Name, _ID)
	, m_TargetObject(nullptr)
	, m_Type(_Type)	
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::SetTarget(CGameObject* _Target)
{
	m_TargetObject = _Target;

	if (nullptr == m_TargetObject)
	{
		SetActive(false);
	}
	else
	{
		if (nullptr == m_TargetObject->GetComponent(m_Type))
		{
			SetActive(false);
		}
		else
		{
			SetActive(true);
		}
	}
}

void ComponentUI::render_title()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(6.f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(6.f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(6.f / 7.0f, 0.7f, 0.7f));

	ImGui::Button(COMPONENT_TYPE_STRING[(UINT)GetComponentType()]);

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}
