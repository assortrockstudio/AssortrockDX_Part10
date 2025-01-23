#pragma once
#include "ComponentUI.h"
class CameraUI :
    public ComponentUI
{
private:
    int     m_SelectedProjType;


public:
    virtual void render_tick() override;

public:
    CameraUI();
    ~CameraUI();
};

