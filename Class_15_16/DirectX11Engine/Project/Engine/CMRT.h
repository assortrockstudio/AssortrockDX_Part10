#pragma once
#include "CEntity.h"

class CMRT :
    public CEntity
{
private:
    Ptr<CTexture>   m_RTTex[8];    
    Vec4            m_ClearColor[8];
    UINT            m_RTCount;    
    Ptr<CTexture>   m_DSTex;

    D3D11_VIEWPORT  m_ViewPort;

public:
    void OMSet();
    void ClearTarget();
    void ClearDepthStencil();
    void Create(Ptr<CTexture>* _RTTex, UINT _RTCount, Ptr<CTexture> _DSTex);

    void SetViewPort(const D3D11_VIEWPORT& _ViewPort) { m_ViewPort = _ViewPort; }
    const D3D11_VIEWPORT& GetViewPort() { return m_ViewPort; }

    void SetClearColor(UINT _Idx, Vec4 _ClearColor) { m_ClearColor[_Idx] = _ClearColor; }

    CLONE_DISABLE(CMRT);
public:   
    CMRT();
    ~CMRT();
};

