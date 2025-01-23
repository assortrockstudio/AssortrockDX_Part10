#pragma once
#include "CRenderComponent.h"

#include "CHeightMapCS.h"
#include "CRaycastCS.h"

class CStructuredBuffer;

struct tRaycastOut
{
    Vec2    Location;
    UINT    Distance;
    int     Success;
};

struct tWeight
{
    float   Weight[8];
};

enum LANDSCAPE_MODE
{
    NONE,
    HEIGHTMAP,
    SPLATING,
};


class CLandScape :
    public CRenderComponent
{
private:
    UINT                    m_FaceX;
    UINT                    m_FaceZ;

    // Brush
    Vec2                    m_BrushScale;
    vector<Ptr<CTexture>>   m_vecBrush;
    UINT                    m_BrushIdx;

    // Tessellation 
    float                   m_MinLevel;
    float                   m_MaxLevel;
    float                   m_MaxLevelRange;
    float                   m_MinLevelRange;

    // HeightMap
    Ptr<CTexture>           m_HeightMap;
    bool                    m_IsHeightMapCreated;
    Ptr<CHeightMapCS>       m_HeightMapCS;

    // WeightMap
    Ptr<CTexture>           m_ColorTex;
    Ptr<CTexture>           m_NormalTex;
    CStructuredBuffer*      m_WeightMap;


    // Raycasting
    Ptr<CRaycastCS>         m_RaycastCS;
    CStructuredBuffer*      m_RaycastOut;
    tRaycastOut             m_Out;

    // LandScape ahem
    LANDSCAPE_MODE          m_Mode;




public:
    void SetFace(UINT _X, UINT _Z);
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap; m_IsHeightMapCreated = false;}
    void AddBrushTexture(Ptr<CTexture> _BrushTex) { m_vecBrush.push_back(_BrushTex); }
    void CreateHeightMap(UINT _Width, UINT _Height);

public:
    void init();
    virtual void finaltick() override;
    virtual void render() override;

private:
    void CreateMesh();
    void CreateComputeShader();
    void CreateTexture();
    void Binding();
    int Raycasting();

public:
    CLONE(CLandScape);

public:
    CLandScape();
    ~CLandScape();
};

