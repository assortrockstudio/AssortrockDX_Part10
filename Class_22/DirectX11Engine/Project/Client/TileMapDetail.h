#pragma once
#include "TMESub.h"
class TileMapDetail :
    public TMESub
{


public:
    virtual void render_tick() override;

public:
    UINT SelectTexture(DWORD_PTR _Selected);


public:
    TileMapDetail();
    ~TileMapDetail();
};

