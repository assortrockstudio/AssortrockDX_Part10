#include "pch.h"
#include "MenuUI.h"

#include <Engine/CEngine.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CScript.h>

#include <Engine/CPathMgr.h>
#include <Scripts/CScriptMgr.h>

#include <Engine/CRenderMgr.h>

#include "CLevelSaveLoad.h"

#include "CImGuiMgr.h"
#include "Inspector.h"
#include "ContentUI.h"
#include "OutlinerUI.h"

MenuUI::MenuUI()
	: EditorUI("Menu", "##Menu")
{
}

MenuUI::~MenuUI()
{
}


void MenuUI::tick()
{
	if (ImGui::BeginMainMenuBar())
	{
        File();

        Level();

        GameObject();

        Asset();
             
        RenderTarget();

		ImGui::EndMainMenuBar();
	}
}

void MenuUI::render_tick(){}

void MenuUI::File()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Exit", "Alt+F4"))
        {
            DestroyWindow(CEngine::GetInst()->GetMainWnd());
        }

        ImGui::EndMenu();
    }
}


void MenuUI::Level()
{
    if (ImGui::BeginMenu("Level"))
    {
        if (ImGui::MenuItem("Save Level"))
        {
            wchar_t Buff[255] = {};

            OPENFILENAME ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = Buff;        // 결과 출력
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = 255;
            ofn.lpstrFilter = L"Level\0*.lv\0All\0*.*";
            ofn.nFilterIndex = 0;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // 탐색창 초기 위치 지정
            wstring strInitPath = CPathMgr::GetInst()->GetContentPath();            
            ofn.lpstrInitialDir = strInitPath.c_str();
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetSaveFileName(&ofn))
            {
                CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
                if (nullptr != pCurLevel)
                {
                    CLevelSaveLoad::SaveLevel(pCurLevel, Buff);
                }
            }
        }

        if (ImGui::MenuItem("Load Level"))
        {
            wchar_t Buff[255] = {};

            OPENFILENAME ofn = {};

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = Buff;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = 255;
            ofn.lpstrFilter = L"Level\0*.lv\0All\0*.*";
            ofn.nFilterIndex = 0;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // 탐색창 초기 위치 지정
            wstring strInitPath = CPathMgr::GetInst()->GetContentPath();            
            ofn.lpstrInitialDir = strInitPath.c_str();
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn))
            {
                CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(Buff);
                ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);
            }
        }

        ImGui::Separator();


        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        bool IsCurLevel = pCurLevel;
        bool IsPlayState = pCurLevel->GetState() == LEVEL_STATE::PLAY;
        bool IsPauseState = pCurLevel->GetState() == LEVEL_STATE::PAUSE;
        bool IsStopState = pCurLevel->GetState() == LEVEL_STATE::STOP;
        

        // Play
        ImGui::BeginDisabled(IsPlayState);
        if (ImGui::MenuItem("Play"))
        {
            ChangeLevelState(LEVEL_STATE::PLAY);

            if (IsStopState)
            {
                CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
                wstring LevelPath = CPathMgr::GetInst()->GetContentPath();
                LevelPath += L"level\\temp.lv";
                CLevelSaveLoad::SaveLevel(pLevel, LevelPath);
            }
        }
        ImGui::EndDisabled();


        // Pause
        ImGui::BeginDisabled(!IsPlayState);
        if (ImGui::MenuItem("Pause"))
        {
            ChangeLevelState(LEVEL_STATE::PAUSE);
        }
        ImGui::EndDisabled();


        ImGui::BeginDisabled(IsStopState);
        if (ImGui::MenuItem("Stop"))
        {
            wstring LevelPath = CPathMgr::GetInst()->GetContentPath();
            LevelPath += L"level\\temp.lv";
            CLevel* pNextLevel = CLevelSaveLoad::LoadLevel(LevelPath);
            ChangeLevel(pNextLevel, LEVEL_STATE::STOP);

            // Inspector 타겟 오브젝트 클리어
            Inspector* pInspector = CImGuiMgr::GetInst()->FindEditorUI<Inspector>("Inspector");
            pInspector->SetTargetObject(nullptr);
        }
        ImGui::EndDisabled();

        ImGui::EndMenu();
    }
}


void MenuUI::GameObject()
{
    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Create EmptyObject"))
        {
            CGameObject* pNewObj = new CGameObject;
            pNewObj->AddComponent(new CTransform);
            pNewObj->SetName(L"Empty GameObject");
            SpawnObject(0, pNewObj);

            Inspector* pInspector = CImGuiMgr::GetInst()->FindEditorUI<Inspector>("Inspector");
            pInspector->SetTargetObject(pNewObj);
        }

        if (ImGui::MenuItem("Register As Prefab"))
        {

        }

		if (ImGui::BeginMenu("Add Component"))
		{
            Inspector* pInspector = CImGuiMgr::GetInst()->FindEditorUI<Inspector>("Inspector");
            CGameObject* pTargetObj = pInspector->GetTargetObject();

            ImGui::BeginDisabled(!pTargetObj);

			for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
			{
				if (ImGui::MenuItem(COMPONENT_TYPE_STRING[i]))
				{
					if (nullptr != pTargetObj)
					{
						switch (COMPONENT_TYPE(i))
						{
						case COMPONENT_TYPE::TRANSFORM:
							pTargetObj->AddComponent(new CTransform);
							break;
						case COMPONENT_TYPE::CAMERA:
							pTargetObj->AddComponent(new CCamera);
							break;
						case COMPONENT_TYPE::COLLIDER2D:
							pTargetObj->AddComponent(new CCollider2D);
							break;
						case COMPONENT_TYPE::COLLIDER3D:

							break;
						case COMPONENT_TYPE::ANIMATOR2D:
							pTargetObj->AddComponent(new CAnimator2D);
							break;
						case COMPONENT_TYPE::ANIMATOR3D:

							break;
						case COMPONENT_TYPE::LIGHT2D:
							pTargetObj->AddComponent(new CLight2D);
							break;
						case COMPONENT_TYPE::LIGHT3D:
							break;
						case COMPONENT_TYPE::MESHRENDER:
							pTargetObj->AddComponent(new CMeshRender);
							break;
						case COMPONENT_TYPE::DECAL:
							break;
						case COMPONENT_TYPE::PARTICLESYSTEM:
							pTargetObj->AddComponent(new CParticleSystem);
							break;
						case COMPONENT_TYPE::TILEMAP:
							pTargetObj->AddComponent(new CTileMap);
							break;
						case COMPONENT_TYPE::LANDSCAPE:
							break;
						}
					}
				}

            }
			
            ImGui::EndDisabled();

            pInspector->SetTargetObject(pTargetObj);

            ImGui::EndMenu();           
		}

        if (ImGui::BeginMenu("Add Script"))
        {
            Inspector* pInspector = CImGuiMgr::GetInst()->FindEditorUI<Inspector>("Inspector");
            CGameObject* pTargetObj = pInspector->GetTargetObject();

			ImGui::BeginDisabled(!pTargetObj);

			vector<wstring> vecScriptName;
			CScriptMgr::GetScriptInfo(vecScriptName);

			for (size_t i = 0; i < vecScriptName.size(); ++i)
			{
                string scriptName = ToString(vecScriptName[i]);

				if (ImGui::MenuItem(scriptName.c_str()))
				{
                    CScript* pNewScript = CScriptMgr::GetScript(vecScriptName[i]);
                    pTargetObj->AddComponent(pNewScript);
                    pInspector->SetTargetObject(pTargetObj);
                    pInspector->SetFocus();
				}
			}

			ImGui::EndDisabled();
            ImGui::EndMenu();
        }



        ImGui::EndMenu();
    }
}


void MenuUI::Asset()
{
    if (ImGui::BeginMenu("Asset"))
    {
        if (ImGui::MenuItem("Create Empty Material"))
        {
            // 재질하나 생성해서 AssetMgr 에 등록
            Ptr<CMaterial> pMtrl = new CMaterial;

            wstring strMtrlKey = GetAssetDefaultName(L"material//Default Material");
            strMtrlKey += L".mtrl";
            CAssetMgr::GetInst()->AddAsset<CMaterial>(strMtrlKey, pMtrl);

            // 재질을 파일로 실제로 저장
            wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
            strFilePath += strMtrlKey;
            pMtrl->Save(strFilePath);


            // 새로운 에셋이 등록되었기 때문에, ContentUI 갱신
            ContentUI* pContent = CImGuiMgr::GetInst()->FindEditorUI<ContentUI>("Content");
            pContent->RenewContent();
        }

        ImGui::Separator();

        ImGui::EndMenu();
    }
}

void MenuUI::RenderTarget()
{
    if (ImGui::BeginMenu("RenderTarget"))
    {
        if (ImGui::BeginMenu("Target Filter"))
        {
            RenderTargetFilter("ColorTarget",    L"ColorTargetTex");
            RenderTargetFilter("NormalTarget",   L"NormalTargetTex");
            RenderTargetFilter("PositionTarget", L"PositionTargetTex");
            RenderTargetFilter("DataTarget",     L"DataTargetTex");
            RenderTargetFilter("DiffuseTarget",  L"DiffuseTargetTex");
            RenderTargetFilter("SpecularTarget", L"SpecularTargetTex");
            RenderTargetFilter("EmissiveTarget", L"EmissiveTargetTex");

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

wstring MenuUI::GetAssetDefaultName(wstring _BaseName)
{
    _BaseName += L" %d";

    wchar_t szKey[255] = {};

    int i = 0;
    while (true)
    {
        wcscpy_s(szKey, 255, _BaseName.c_str());
        swprintf_s(szKey, 255, szKey, i++);

        if (nullptr == CAssetMgr::GetInst()->FindAsset<CMaterial>(szKey))
        {
            break;
        }
    }

    return szKey;
}

void MenuUI::RenderTargetFilter(const string& _MenuItemName, const wstring& _TargetTexKey)
{
    Ptr<CTexture> pOutputTargetTex = nullptr;
    bool IsTarget = false;

    if (CRenderMgr::GetInst()->IsOutputTarget())
    {
        pOutputTargetTex = CRenderMgr::GetInst()->GetOutputTarget();
    }

    if (nullptr != pOutputTargetTex && pOutputTargetTex->GetKey() == _TargetTexKey)
        IsTarget = true;
    else
        IsTarget = false;

    if (ImGui::MenuItem(_MenuItemName.c_str(), nullptr, &IsTarget))
    {
        if (IsTarget)
            CRenderMgr::GetInst()->SetOutputTarget(true, CAssetMgr::GetInst()->FindAsset<CTexture>(_TargetTexKey));
        else
            CRenderMgr::GetInst()->SetOutputTarget(false, nullptr);
    }
}
