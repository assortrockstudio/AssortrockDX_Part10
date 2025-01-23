#pragma once
#include "ComponentUI.h"
class TileMapUI :
    public ComponentUI
{

public:
    virtual void render_tick() override;

public:
    TileMapUI();
    ~TileMapUI();
};

