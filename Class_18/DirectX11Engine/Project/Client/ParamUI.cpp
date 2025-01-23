#include "pch.h"
#include "ParamUI.h"

#include "ImGui/imgui.h"
#include "CImGuiMgr.h"
#include "ListUI.h"

UINT ParamUI::g_ParmUIID = 0;
int ParamUI::g_DescWidth = 0;

EditorUI*       ParamUI::g_Inst = nullptr;
UI_DELEGATE1	ParamUI::g_Delegate_1 = nullptr;


int ParamUI::Param_DragInt(const string& _strName, int* _pInOut, int _Speed)
{
    // 파라미터 이름
    ImGui::Text(_strName.c_str());
    ImGui::SameLine(g_DescWidth + 5);

    // 키값 생성
    char key[255] = {};
    sprintf_s(key, 255, "##DragInt_%d", g_ParmUIID++);        
    if (ImGui::DragInt(key, _pInOut, _Speed))
    {
        return true;
    }
        
    return false;
}

int ParamUI::Param_DragFloat(const string& _strName, float* _pInOut, float _Speed)
{
    // 파라미터 이름
    ImGui::Text(_strName.c_str());    
    ImGui::SameLine(g_DescWidth + 5);

    // 키값 생성
    char key[255] = {};
    sprintf_s(key, 255, "##DragFloat_%d", g_ParmUIID++);

    if (ImGui::DragFloat(key, _pInOut, _Speed))
    {
        return true;
    }

    return false;
}

int ParamUI::Param_DragVec2(const string& _strName, Vec2* _pInOut, float _Speed)
{
    // 파라미터 이름
    ImGui::Text(_strName.c_str());
    ImGui::SameLine(g_DescWidth + 5);

    // 키값 생성
    char key[255] = {};
    sprintf_s(key, 255, "##DragVec2_%d", g_ParmUIID++);

    float arr[2] = { _pInOut->x, _pInOut->y };

    if (ImGui::DragFloat2(key, arr, _Speed))
    {
        _pInOut->x = arr[0];
        _pInOut->y = arr[1];

        return true;
    }
}

int ParamUI::Param_DragVec4(const string& _strName, Vec4* _pInOut, float _Speed)
{
    // 파라미터 이름
    ImGui::Text(_strName.c_str());
    ImGui::SameLine(g_DescWidth + 5);

    // 키값 생성
    char key[255] = {};
    sprintf_s(key, 255, "##DragVec4_%d", g_ParmUIID++);

    if (ImGui::DragFloat4(key, *_pInOut, _Speed))
    {
        return true;
    }

    return false;
}

int ParamUI::Param_DragMat(const string& _strName, Matrix* _pInOut, float _Speed)
{

    return false;
}


int ParamUI::Param_Tex(const string& _strName, Ptr<CTexture>& _Tex)
{
    int btnClicked = false;

    // 파라미터 이름
    ImGui::Text(_strName.c_str());
    ImGui::SameLine(g_DescWidth + 5);

    // 키값 생성
    char key[255] = {};
    sprintf_s(key, 255, "##InputText_%d", g_ParmUIID++);

    // 텍스쳐 이름 출력
    string strTexName;
    if(nullptr != _Tex)
        strTexName = ToString(_Tex->GetKey());

    ImGui::SetNextItemWidth(200);
    ImGui::InputText(key, (char*)strTexName.c_str(), strTexName.capacity(), ImGuiInputTextFlags_ReadOnly);

    // DragDrop
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
                _Tex = (CTexture*)pAsset.Get();
            }
        }

        ImGui::EndDragDropTarget();
    }


    ImGui::SameLine();
    // ListUI 버튼 추가
    sprintf_s(key, 255, "##TextureBtn_%d", g_ParmUIID++);
    if (ImGui::Button(key, ImVec2(18, 18)))
    {
        // ListUI 를 활성화한다
        ListUI* pListUI = CImGuiMgr::GetInst()->FindEditorUI<ListUI>("List");

        if (nullptr != pListUI && !pListUI->IsActive())
        {
            pListUI->SetModal(true);
            pListUI->SetActive(true);
            pListUI->SetFocus();

            vector<string> vecNames;
            CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecNames);
            pListUI->AddItem(vecNames);
            
            if(g_Inst && g_Delegate_1)
                pListUI->RegisterDBClickDelegate(g_Inst, g_Delegate_1);

            g_Inst = nullptr;
            g_Delegate_1 = nullptr;
        }

        btnClicked = true;
    }
 

    // Image 출력
    ImTextureID TexID = (nullptr == _Tex) ? nullptr : _Tex->GetSRV().Get(); 
    ImVec4 BorderColor = (nullptr == _Tex) ? ImVec4(1.f, 1.f, 1.f, 1.f) : ImVec4(1.f, 1.f, 1.f, 0.f);

    ImGui::Image( TexID
                , ImVec2(150.f, 150.f)
                , ImVec2(0.0f, 0.0f)
                , ImVec2(1.0f, 1.0f)
                , ImGui::GetStyleColorVec4(ImGuiCol_Text)
                , BorderColor);

    return btnClicked;
}
