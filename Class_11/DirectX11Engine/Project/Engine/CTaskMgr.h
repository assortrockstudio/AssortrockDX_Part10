#pragma once
#include "singleton.h"

class CTaskMgr :
    public CSingleton<CTaskMgr>
{
    SINGLE(CTaskMgr);
private:
    vector<tTask>           m_vecTasks;
    vector<CGameObject*>    m_vecDead;
    bool                    m_ObjectEvent;

public:
    bool IsObjectChanged() { return m_ObjectEvent; }

public:
    void tick();
    void AddTask(const tTask& _Task) { m_vecTasks.push_back(_Task); }

private:
    void ClearDeadObject();
    void ExecuteTask(tTask& _Task);
    void ClearEvent() 
    {
        m_ObjectEvent = false;
    }
};

