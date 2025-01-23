#include "pch.h"
#include "AssetUI.h"

AssetUI::AssetUI(const string& _Name, const string& _ID, ASSET_TYPE _Type)
	: EditorUI(_Name, _ID)
	, m_Type(_Type)
{
}

AssetUI::~AssetUI()
{
}


void AssetUI::SetTarget(Ptr<CAsset> _Target)
{
	if (m_TargetAsset == _Target)
		return;

	m_TargetAsset = _Target;

	TargetChanged();
}

void AssetUI::render_title()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3.f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3.f / 7.0f, 0.7f, 0.7f));

	ImGui::Button(ASSET_TYPE_STRING[(UINT)GetAssetType()]);

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}
