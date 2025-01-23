#pragma once
#include "AssetUI.h"

class TextureUI :
    public AssetUI
{
public:
    virtual void render_tick() override;

public:
    TextureUI();
    ~TextureUI();
};

