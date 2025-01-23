#pragma once


typedef void (*UI_CALLBACK)(void);

class EditorUI;
typedef UINT(EditorUI::* UI_DELEGATE)(void);
typedef UINT(EditorUI::* UI_DELEGATE1)(DWORD_PTR);
typedef UINT(EditorUI::* UI_DELEGATE2)(DWORD_PTR, DWORD_PTR);

class CImGuiMgr :
    public CSingleton<CImGuiMgr>
{
    SINGLE(CImGuiMgr);
private:
    HWND                    m_hMainWnd;
    map<string, EditorUI*>  m_mapUI;
    bool                    m_bDemo;
    HANDLE                  m_hNotify;

public:
    template<typename T>
    T* FindEditorUI(const string& _strName);

public:
    int init(HWND _hWnd);
    void tick();

private:
    void CreateEditorUI();
    void ObserveContent();
};

template<typename T>
inline T* CImGuiMgr::FindEditorUI(const string& _strName)
{
    map<string, EditorUI*>::iterator iter = m_mapUI.find(_strName);

    if (iter == m_mapUI.end())
        return nullptr;

    T* pUI = dynamic_cast<T*>(iter->second);

    return pUI;
}
