#pragma once
#include <Engine\singleton.h>


class CEditorMgr :
    public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);
private:
    vector<CGameObject*>     m_vecEditorObj;


public:
    void init();
    void tick();
};

