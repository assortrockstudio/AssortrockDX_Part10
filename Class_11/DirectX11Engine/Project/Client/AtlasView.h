#pragma once
#include "TMESub.h"

class AtlasView :
    public TMESub
{
private:
    float   m_DefaultSize;
    float   m_AtlasScale;

    Vec2    m_ImageRectMin;
    int     m_ImgIdx;


public:
    int GetImgIdx() { return m_ImgIdx; }

public:
    virtual void render_tick() override;

private:
    void MouseCheck();
    void DrawSelectedTile();

public:
    AtlasView();
    ~AtlasView();
};

