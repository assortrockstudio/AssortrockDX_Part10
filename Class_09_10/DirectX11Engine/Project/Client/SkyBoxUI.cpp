#include "pch.h"
#include "SkyBoxUI.h"

#include <Engine/CSkyBox.h>

SkyBoxUI::SkyBoxUI()
	: ComponentUI("SkyBox", "##SkyBoxUI", COMPONENT_TYPE::SKYBOX)
{
}

SkyBoxUI::~SkyBoxUI()
{
}

void SkyBoxUI::render_tick()
{
	render_title();

	CSkyBox* pSkyBox = GetTarget()->SkyBox();
	

    // SkyBoxType
    ImGui::Text("SkyBox Type");
    ImGui::SameLine(100);    

	SKYBOX_TYPE Type = pSkyBox->GetSkyBoxType();

    const char* items[2] = {};
    items[0] = "SPHERE";
    items[1] = "CUBE";

    if (ImGui::BeginCombo("##SkyBoxType", items[(UINT)Type], 0))
    {
        for (int i = 0; i < 2; ++i)
        {
            const bool is_selected = ((UINT)Type == i);
            if (ImGui::Selectable(items[i], is_selected))
            {
                pSkyBox->SetSkyBoxType((SKYBOX_TYPE)i);
            }
        }
        ImGui::EndCombo();
    }

    // SkyBox Texture
    Ptr<CTexture> pTex = pSkyBox->GetSkyBoxTexture();
    ImTextureID TexID = 0;
    string strTexKey;

    if (nullptr != pTex)
    {
        TexID = pTex->GetSRV().Get();
        strTexKey = ToString(pTex->GetKey());
    }
    else
    {
        strTexKey = "None";
    }
     
    ImGui::Text("SkyBox Texture");
    ImGui::SameLine(100);

    // Texture 이름   
    ImGui::InputText("##TextureUIName", (char*)strTexKey.c_str(), strTexKey.capacity(), ImGuiInputTextFlags_ReadOnly);

   
    // 이미지
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right

    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    ImGui::Image(TexID, ImVec2(150.f, 150.f), uv_min, uv_max, ImGui::GetStyleColorVec4(ImGuiCol_Text), border_col);



    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content");

        if (nullptr != payload)
        {
            DWORD_PTR dwData = 0;
            memcpy(&dwData, payload->Data, payload->DataSize);

            Ptr<CAsset> pAsset = (CAsset*)dwData;
            if (pAsset->GetAssetType() == ASSET_TYPE::TEXTURE)
            {
                pSkyBox->SetSkyBoxTexture((CTexture*)pAsset.Get());
            }
        }

        ImGui::EndDragDropTarget();
    }
}