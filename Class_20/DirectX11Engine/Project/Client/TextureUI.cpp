#include "pch.h"
#include "TextureUI.h"

#include <Engine/CTexture.h>

TextureUI::TextureUI()
	: AssetUI("TextureUI", "##TextureUI", ASSET_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{
}

void TextureUI::render_tick()
{
	render_title();

    Ptr<CTexture> pTexture = dynamic_cast<CTexture*>(GetTarget().Get());
    assert(pTexture.Get());

    // Texture 이름
    ImGui::Text("Name");
    ImGui::SameLine();

    string strTexKey = ToString(pTexture->GetKey());
    ImGui::InputText("##TextureUIName", (char*)strTexKey.c_str(), strTexKey.capacity(), ImGuiInputTextFlags_ReadOnly);

    // 이미지
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    
    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    ImGui::Image((ImTextureID)pTexture->GetSRV().Get(), ImVec2(150.f, 150.f), uv_min, uv_max, ImGui::GetStyleColorVec4(ImGuiCol_Text), border_col);


    // 해상도
    ImGui::Text("Resolution");

    int width = pTexture->GetWidth();
    int height = pTexture->GetHeight();

    ImGui::Text("Width");
    ImGui::SameLine(100);
    ImGui::InputInt("##TextureWidth", &width, 0, 100, ImGuiInputTextFlags_ReadOnly);

    ImGui::Text("Height");
    ImGui::SameLine(100);
    ImGui::InputInt("##TextureHeight", &height, 0, 100, ImGuiInputTextFlags_ReadOnly);    
}