#pragma once
#include "ComponentUI.h"

class SkyBoxUI :
    public ComponentUI
{
private:
    

public:
    virtual void render_tick() override;

public:
    SkyBoxUI();
    ~SkyBoxUI();
};

