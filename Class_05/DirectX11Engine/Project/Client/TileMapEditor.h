#pragma once
#include "EditorUI.h"
#include <Engine/CTileMap.h>

enum TME_SUB
{
    TILEVIEW,
    ATLASVIEW,
    DETAIL,
};

class TileMapEditor :
    public EditorUI
{
private:
    class TMESub*   m_Sub[3];
    bool            m_PrevActive[3];

    CTileMap*       m_TileMap;


public:
    void SetTileMap(CTileMap* _TileMapComponent);
    TMESub* GetSubUI(TME_SUB _Sub) { return m_Sub[_Sub]; }


public:
    virtual void render_tick() override;
    virtual void Activate() override;
    virtual void Deactivate() override;




public:
    TileMapEditor();
    ~TileMapEditor();
};

