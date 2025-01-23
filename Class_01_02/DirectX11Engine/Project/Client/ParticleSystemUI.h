#pragma once
#include "ComponentUI.h"
class ParticleSystemUI :
    public ComponentUI
{

public:
    virtual void render_tick() override;

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

