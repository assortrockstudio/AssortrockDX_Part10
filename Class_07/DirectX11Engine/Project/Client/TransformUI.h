#pragma once
#include "ComponentUI.h"

class TransformUI :
    public ComponentUI
{
private:
   

public:
    virtual void render_tick() override;

public:
    TransformUI();
    ~TransformUI();
};

