#pragma once
#include "CRenderComponent.h"


struct tTileInfo
{
    UINT ImgIdx;
    Vec3 vPadding;
};

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    UINT                m_Row;          // Tile Row Count
    UINT                m_Col;          // Tile Colum Count
    Vec2                m_TileEachSize; // 각 타일 1개의 크기

    Ptr<CTexture>       m_Atlas;
    UINT                m_AtlasMaxRow;
    UINT                m_AtlasMaxCol;
    Vec2                m_AtlasTileEachSize;
    Vec2                m_AtlasResolution;

    vector<tTileInfo>   m_vecTileInfo;
    CStructuredBuffer*  m_TileBuffer;

    Ptr<CTexture>       m_CaptureTex;
    Vec2                m_CaptureTexMaxResolution;
    bool                m_EditorCapture;

public:
    void SetRowCol(UINT _Row, UINT _Col);
    void SetAtlasTexture(Ptr<CTexture> _Tex);
    void SetAtlasTileSize(Vec2 _TileSize);
    void SetTileEachSize(Vec2 _vSize);
    void SetCapture(bool _Capture) { m_EditorCapture = _Capture; }
    void SetImgIndex(int _Row, int _Col, int _imgidx);


    UINT GetRow() { return m_Row; }
    UINT GetColumn() { return m_Col; }
    Vec2 GetTileSize() { return m_TileEachSize; }

    Ptr<CTexture> GetAtlas() { return m_Atlas; }
    UINT GetAtlasMaxRow() { return m_AtlasMaxRow; }
    UINT GetAtlasMaxColumn() { return m_AtlasMaxCol; }
    Vec2 GetAtlasTileSize() { return m_AtlasTileEachSize; }
    Ptr<CTexture> GetCaptureTexture() { return m_CaptureTex; }

public:
    virtual void begin() override;
    virtual void finaltick() override;
    virtual void render() override;

private:
    void CaptureRender();
    void CheckCaptureTexture();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CTileMap);
    CTileMap();
    CTileMap(const CTileMap& _Other);
    ~CTileMap();
};

