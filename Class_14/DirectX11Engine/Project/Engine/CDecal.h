#pragma once

#include "CRenderComponent.h"

class CDecal :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_OutputTex;
    bool            m_AsLight;
    float           m_LightPow;

public:
    void SetOutputTexture(Ptr<CTexture> _Tex) { m_OutputTex = _Tex; }
    void SetAsLight(bool _Light) { m_AsLight = _Light; }
    void SetDecalPow(float _Pow) 
    {         
        m_LightPow = _Pow; 

        if (m_LightPow < 0.f)
            m_LightPow = 0.f;
        else if (1.f < m_LightPow)
            m_LightPow = 1.f;
    }

public:
    virtual void finaltick() override;
    virtual void render() override;

public:
    CLONE(CDecal);

public:
    CDecal();
    ~CDecal();
};

