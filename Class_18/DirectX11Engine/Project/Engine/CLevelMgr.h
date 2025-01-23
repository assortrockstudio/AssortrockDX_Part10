#pragma once
#include "singleton.h"

class CLevel;

class CLevelMgr :
    public CSingleton<CLevelMgr>
{
    SINGLE(CLevelMgr)
private:
    CLevel*     m_CurLevel;

public:
    CLevel* GetCurrentLevel() { return m_CurLevel; }

public:
    void init();
    void tick();    

private:
    void ChangeLevel(CLevel* _NextLevel);

    friend class CTaskMgr;
};

