#pragma once
#include "AssetUI.h"
class ComputeShaderUI :
    public AssetUI
{
public:
    virtual void render_tick() override;

public:
    ComputeShaderUI();
    ~ComputeShaderUI();
};

