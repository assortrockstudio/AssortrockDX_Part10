#pragma once
#include "ComponentUI.h"

class CScript;

class ScriptUI :
    public ComponentUI
{
private:
    CScript* m_TargetScript;


public:
    void SetScript(CScript* _Script);

public:
    virtual void render_tick() override;


private:
    void render_scriptname();

public:
    ScriptUI();
    ~ScriptUI();
};

