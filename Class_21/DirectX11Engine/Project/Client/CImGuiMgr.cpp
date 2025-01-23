#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CDevice.h>
#include <Engine/CKeyMgr.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "ParamUI.h"

#include "Inspector.h"
#include "ListUI.h"
#include "ContentUI.h"
#include "OutlinerUI.h"
#include "MenuUI.h"

#include "TileMapEditor.h"
#include "TileView.h"
#include "AtlasView.h"
#include "TileMapDetail.h"



CImGuiMgr::CImGuiMgr()
    : m_hMainWnd(nullptr)
    , m_bDemo(false)
    , m_hNotify(nullptr)
{

}

CImGuiMgr::~CImGuiMgr()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Delete EditorUI
    Safe_Del_Map(m_mapUI);
}

int CImGuiMgr::init(HWND _hWnd)
{
    m_hMainWnd = _hWnd;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(m_hMainWnd))
        return E_FAIL;

    if (!ImGui_ImplDX11_Init(DEVICE, CONTEXT))
        return E_FAIL;

    CreateEditorUI();

    
    // ContentFolder 감시 등록
    wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

    m_hNotify = FindFirstChangeNotification(strContentPath.c_str(), true
             , FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
             | FILE_ACTION_ADDED | FILE_ACTION_REMOVED);

	return S_OK;
}

void CImGuiMgr::tick()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    // Parameter UI 들의 아이디 리셋
    ParamUI::ResetParamID();

    // Demo Window 활성화
    if (KEY_TAP(KEY::NUM9)) { m_bDemo ? m_bDemo = false : m_bDemo = true; }      
    if (m_bDemo) ImGui::ShowDemoWindow();
    
    for (const auto& pair : m_mapUI)
    {
        if(pair.second->IsActive())
            pair.second->tick();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    // 알림 확인
    ObserveContent();
}

void CImGuiMgr::CreateEditorUI()
{
    EditorUI* pUI = nullptr;

    pUI = new Inspector;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ListUI;
    pUI->SetActive(false);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ContentUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new OutlinerUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new MenuUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new TileView;
    pUI->SetActive(false);
    pUI->SetUIMove(false);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new AtlasView;
    pUI->SetActive(false);    
    pUI->SetUIMove(false);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new TileMapDetail;
    pUI->SetActive(false);
    pUI->SetUIMove(false);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new TileMapEditor;
    pUI->SetActive(false);
    pUI->SetMenu(true);
    pUI->SetUIMove(true);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));
}

void CImGuiMgr::ObserveContent()
{
    DWORD dwWaitStatus = WaitForSingleObject(m_hNotify, 0);

    if (WAIT_OBJECT_0 == dwWaitStatus)
    {
        ContentUI* pContentUI = FindEditorUI<ContentUI>("Content");
        pContentUI->ReloadContent();
        pContentUI->RenewContent();

        FindNextChangeNotification(m_hNotify);
    }
}
