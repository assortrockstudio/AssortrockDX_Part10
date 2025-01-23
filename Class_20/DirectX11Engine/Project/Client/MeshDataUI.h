#pragma once
#include "AssetUI.h"
class MeshDataUI :
    public AssetUI
{
public:
    virtual void render_tick() override;

public:
    MeshDataUI();
    ~MeshDataUI();
};

