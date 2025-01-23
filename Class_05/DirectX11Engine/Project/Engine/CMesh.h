#pragma once
#include "CAsset.h"

class CMesh :
    public CAsset
{
private:   
    ComPtr<ID3D11Buffer>    m_VB;
    D3D11_BUFFER_DESC       m_VBDesc;
    UINT                    m_VtxCount;
    Vtx*                    m_VtxSysMem;

    ComPtr<ID3D11Buffer>    m_IB;
    D3D11_BUFFER_DESC       m_IBDesc;
    UINT                    m_IdxCount;
    UINT*                   m_IdxSysMem;

public:
    UINT GetVertexCount() { return m_VtxCount; }
    UINT GetIndexCount() { return m_IdxCount; }

private:
    void Binding();
    virtual int Load(const wstring& _FilePath) override { return S_OK; }
    virtual int Save(const wstring& _FilePath) override { return S_OK; }

public:
    int Create(Vtx* _VtxSysMem, size_t VtxCount, UINT* _IdxSysMem, size_t _IdxCount);    
    void render();
    void render_particle(UINT _instance);

    CLONE_DISABLE(CMesh);
public:
    CMesh(bool _bEngine = false);
    ~CMesh();
};

