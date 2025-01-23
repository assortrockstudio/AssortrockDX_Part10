#pragma once
#include "AssetUI.h"
class PrefabUI :
    public AssetUI
{
public:
    virtual void render_tick() override;

public:
    PrefabUI();
    ~PrefabUI();
};

