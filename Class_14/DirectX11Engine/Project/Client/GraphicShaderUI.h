#pragma once
#include "AssetUI.h"
class GraphicShaderUI :
    public AssetUI
{
public:
    virtual void render_tick() override;

public:
    GraphicShaderUI();
    ~GraphicShaderUI();
};

