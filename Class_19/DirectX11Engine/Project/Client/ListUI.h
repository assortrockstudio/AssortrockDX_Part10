#pragma once
#include "EditorUI.h"

class ListUI :
    public EditorUI
{
private:
    vector<string>  m_vecList;
    int             m_SelectedIdx;

    UI_CALLBACK     m_CallBack;

    EditorUI*       m_Inst;
    UI_DELEGATE1    m_Delegate;

public:
    const string& GetSelected() { return m_vecList[m_SelectedIdx]; }

    void AddItem(const string& _Item) { m_vecList.push_back(_Item); }
    void AddItem(const vector<string>& _vecItem) { m_vecList.insert(m_vecList.end(), _vecItem.begin(), _vecItem.end()); }

    void RegisterDBClickCallBack(UI_CALLBACK _CallBack) { m_CallBack = _CallBack; }
    void RegisterDBClickDelegate(EditorUI* _Inst, UI_DELEGATE1 _Func)
    {
        m_Inst = _Inst;
        m_Delegate = _Func;
    }

public:
    virtual void render_tick() override;
    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    ListUI();
    ~ListUI();
};

