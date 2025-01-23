#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnim2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim2D*>  m_mapAnim;
    CAnim2D*                m_CurAnim;
    bool                    m_Repeat;


public:
    void CreateAnimation(const wstring& _AnimName, Ptr<CTexture> _AltasTex, Vec2 _LeftTopPixelPos, Vec2 _vSlicePixelSize, Vec2 _BackgroundSize, int _FrameCount, UINT _FPS);
    CAnim2D* FindAnimation(const wstring& _AnimName);
    void LoadAnimation(const wstring& _strRelativePath);

    void Play(const wstring& _strAnimName, bool _Repeat);

public:
    virtual void finaltick() override;
    void Binding();

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CAnimator2D)
public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _Other);
    ~CAnimator2D();
};

