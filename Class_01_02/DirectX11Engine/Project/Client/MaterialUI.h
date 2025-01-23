#pragma once
#include "AssetUI.h"

class MaterialUI :
    public AssetUI
{
private:
    UINT        m_ParamDescMaxWidth;

    TEX_PARAM   m_TargetParam;

public:
    virtual void render_tick() override;
    virtual void TargetChanged() override;

    UINT SelectTexture(DWORD_PTR _Selected);

public:
    MaterialUI();
    ~MaterialUI();
};

