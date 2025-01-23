#pragma once
#include "AssetUI.h"

class MeshUI :
    public AssetUI
{
public:
    virtual void render_tick() override;

public:
    MeshUI();
    ~MeshUI();
};

