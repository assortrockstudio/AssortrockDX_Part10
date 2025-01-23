#pragma once
#include "CComputeShader.h"

#include "CTexture.h"

class CSetColorCS :
    public CComputeShader
{
private:
    Ptr<CTexture>   m_TargetTex;
    Vec4            m_ClearColor;


public:
    void SetTargetTexture(Ptr<CTexture> _Tex) { m_TargetTex = _Tex; }
    void SetClearColor(Vec3 _Color) { m_ClearColor = _Color; m_ClearColor.w = 1.f;}

public:
    virtual int Binding() override;
    virtual void CalculateGroupNum()override;
    virtual void Clear() override;


public:
    CSetColorCS();
    ~CSetColorCS();
};

