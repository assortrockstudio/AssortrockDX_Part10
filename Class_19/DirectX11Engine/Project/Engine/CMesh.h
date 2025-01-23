#pragma once
#include "CAsset.h"

#include "CFBXLoader.h"

struct tIndexInfo
{
    ComPtr<ID3D11Buffer>    pIB;
    D3D11_BUFFER_DESC       tIBDesc;
    UINT				    iIdxCount;
    void*                   pIdxSysMem;
};


class CMesh :
    public CAsset
{
private:   
    ComPtr<ID3D11Buffer>    m_VB;
    D3D11_BUFFER_DESC       m_VBDesc;
    UINT                    m_VtxCount;
    Vtx*                    m_VtxSysMem;

    // 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결
    vector<tIndexInfo>		m_vecIdxInfo;

public:
    UINT GetVertexCount() { return m_VtxCount; }
    UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }

public:
    static CMesh* CreateFromContainer(CFBXLoader& _loader);
    int Create(Vtx* _VtxSysMem, size_t VtxCount, UINT* _IdxSysMem, size_t _IdxCount);    
    void render(UINT _iSubset);
    void render_particle(UINT _instance);


private:
    void Binding(UINT _Subset);

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

    CLONE_DISABLE(CMesh);
public:
    CMesh(bool _bEngine = false);
    ~CMesh();
};

