#pragma once
#include "AssetUI.h"
class SoundUI :
    public AssetUI
{
public:
    virtual void render_tick() override;

public:
    SoundUI();
    ~SoundUI();
};

