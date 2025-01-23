#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    SRV_ONLY,
    SRV_UAV,
};

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB_Main;           // Main 버퍼 (Binding)
    ComPtr<ID3D11Buffer>                m_SB_Write;          // 쓰기용 버퍼(SysMem -> GPU)
    ComPtr<ID3D11Buffer>                m_SB_Read;           // 읽기용 버퍼(GPU -> Sysmem)

    D3D11_BUFFER_DESC                   m_Desc;         // 버퍼정보 구조체

    ComPtr<ID3D11ShaderResourceView>    m_SRV;          // Shader Resource View
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;          // Unordered Access View

    UINT                                m_ElementSize;  // 버퍼 각 요소 크기
    UINT                                m_ElementCount; // 버퍼의 요소 개수

    SB_TYPE                             m_Type;         // SRV Only or Both
    bool                                m_bSysMemMove;  // System Memory Move Possible

    UINT                                m_SRV_Register;
    UINT                                m_UAV_Register;

public:
    UINT GetElementSize() { return m_ElementSize; }
    UINT GetElementCount() { return m_ElementCount; }

    int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove, void* _InitialData = nullptr);
    void SetData(void* _pData, UINT _ElementCount = 0);
    void GetData(void* _pDest, UINT _ElementCount = 0);
    void ClearData();

    void Binding(UINT _RegisterNum);
    void Binding_CS_SRV(UINT _RegisterNum);
    void Binding_CS_UAV(UINT _RegisterNum);

    void Clear_SRV();
    void Clear_UAV();


    CLONE(CStructuredBuffer);
public:
    CStructuredBuffer();
    CStructuredBuffer(const CStructuredBuffer& _Other);
    ~CStructuredBuffer();
};

