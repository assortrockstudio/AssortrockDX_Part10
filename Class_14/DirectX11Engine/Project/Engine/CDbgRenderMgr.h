#pragma once
#include "singleton.h"


class CGameObject;

class CDbgRenderMgr :
    public CSingleton<CDbgRenderMgr>
{
    SINGLE(CDbgRenderMgr);
private:
    list<tDebugShapeInfo>   m_ShapeInfo;
    CGameObject*            m_DebugRenderObj;

public:
    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_ShapeInfo.push_back(_info); }
    void render();
};

