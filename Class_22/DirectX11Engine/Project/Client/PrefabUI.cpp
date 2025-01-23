#include "pch.h"
#include "PrefabUI.h"

#include <Engine/CPrefab.h>


PrefabUI::PrefabUI()
	: AssetUI("PrefabUI", "##PrefabUI", ASSET_TYPE::PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::render_tick()
{
	render_title();

    Ptr<CPrefab> pPrefab = dynamic_cast<CPrefab*>(GetTarget().Get());
    assert(pPrefab.Get());

    // ÀÌ¸§
    ImGui::Text("Name");
    ImGui::SameLine();

    string strKey = ToString(pPrefab->GetKey());
    ImGui::InputText("##PrefabUIName", (char*)strKey.c_str(), strKey.capacity(), ImGuiInputTextFlags_ReadOnly);

}