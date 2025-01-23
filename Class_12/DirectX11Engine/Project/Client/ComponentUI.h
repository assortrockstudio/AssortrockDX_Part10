#pragma once
#include "EditorUI.h"

#include <Engine/CGameObject.h>

class ComponentUI :
    public EditorUI
{
private:
    CGameObject*            m_TargetObject;
    const COMPONENT_TYPE    m_Type;

public:
    void SetTarget(CGameObject* _Target);
    CGameObject* GetTarget() { return m_TargetObject; }

    COMPONENT_TYPE GetComponentType() { return m_Type; }



public:
    virtual void render_tick() = 0;

protected:
    void render_title();

public:
    ComponentUI(const string& _Name, const string& _ID, COMPONENT_TYPE _Type);    
    ~ComponentUI();
};

