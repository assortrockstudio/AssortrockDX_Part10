#include "pch.h"
#include "ComputeShaderUI.h"

#include <Engine/CComputeShader.h>

ComputeShaderUI::ComputeShaderUI()
	: AssetUI("ComputeShaderUI", "##ComputeShaderUI", ASSET_TYPE::COMPUTE_SHADER)
{
}

ComputeShaderUI::~ComputeShaderUI()
{
}

void ComputeShaderUI::render_tick()
{
	render_title();

    Ptr<CComputeShader> pPrefab = dynamic_cast<CComputeShader*>(GetTarget().Get());
    assert(pPrefab.Get());

    // ÀÌ¸§
    ImGui::Text("Name");
    ImGui::SameLine();

    string strKey = ToString(pPrefab->GetKey());
    ImGui::InputText("##ComputeShaderUIName", (char*)strKey.c_str(), strKey.capacity(), ImGuiInputTextFlags_ReadOnly);

}