#pragma once
#include "EditorUI.h"

#include "TileMapEditor.h"
#include <Engine/CTileMap.h>

class TMESub :
    public EditorUI
{
private:
    TileMapEditor*    m_Owner;
    CTileMap*         m_TileMap;

public:
    void SetTileMap(CTileMap* _TileMap); 
    CTileMap* GetTileMap() { return m_TileMap; }

    TileMapEditor* GetOwner() { return m_Owner; }

    class AtlasView* GetAtlasView();
    class TileView* GetTileView();
    class TileMapDetail* GetDetailView();

public:
    TMESub(const string& _Name, const string& _ID);
    ~TMESub();

    friend class TileMapEditor;
};

