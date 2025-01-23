#pragma once
#include "ComponentUI.h"
class Collider2DUI :
    public ComponentUI
{

public:
    virtual void render_tick() override;

public:
    Collider2DUI();
    ~Collider2DUI();
};

