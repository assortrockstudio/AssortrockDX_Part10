#pragma once
#include "CEntity.h"

struct tAnim2DFrm
{
    Vec2    LeftTop;
    Vec2    SliceSize;
    Vec2    Offset;   

    float   Duration;
    int     padding[3];
};

class CTexture;
class CAnimator2D;

class CAnim2D :
    public CEntity
{
private:
    CAnimator2D*        m_Animator;
    vector<tAnim2DFrm>  m_vecFrm;
    Vec2                m_BackgroundSize;

    int                 m_CurIdx;
    bool                m_Finish;

    float               m_Time;

    Ptr<CTexture>       m_AtlasTex;

public:
    bool IsFinish() { return m_Finish; }
    void Reset() { m_CurIdx = 0; m_Time = 0; m_Finish = false; }


public:
    void Create(Ptr<CTexture> _AltasTex, Vec2 _LeftTopPixelPos, Vec2 _vSlicePixelSize, Vec2 _BackgroundSize, int _FrameCount, UINT _FPS);

    void Save(const wstring& _RelativeFolerPath);
    void Load(const wstring& _RelativePath);

public:
    void finaltick();
    void Binding();
    static void Clear();


    void SaveToLevelFile(FILE* _File);
    void LoadFromLevelFile(FILE* _File);


    CLONE(CAnim2D);
public:
    CAnim2D();
    CAnim2D(const CAnim2D& _Other);
    ~CAnim2D();

    friend class CAnimator2D;
};

