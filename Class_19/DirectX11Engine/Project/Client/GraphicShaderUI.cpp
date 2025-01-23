#include "pch.h"
#include "GraphicShaderUI.h"

#include <Engine/CGraphicShader.h>

GraphicShaderUI::GraphicShaderUI()
	: AssetUI("GraphicShaderUI", "##GraphicShaderUI", ASSET_TYPE::GRAPHICS_SHADER)
{
}

GraphicShaderUI::~GraphicShaderUI()
{
}

void GraphicShaderUI::render_tick()
{
	render_title();

    Ptr<CGraphicShader> pPrefab = dynamic_cast<CGraphicShader*>(GetTarget().Get());
    assert(pPrefab.Get());

    // ÀÌ¸§
    ImGui::Text("Name");
    ImGui::SameLine();

    string strKey = ToString(pPrefab->GetKey());
    ImGui::InputText("##GraphicShaderUIName", (char*)strKey.c_str(), strKey.capacity(), ImGuiInputTextFlags_ReadOnly);

}