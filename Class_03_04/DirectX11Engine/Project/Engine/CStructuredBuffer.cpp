#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
    : m_Desc{}
    , m_ElementSize(0)
    , m_ElementCount(0)
    , m_Type(SB_TYPE::SRV_ONLY)
    , m_SRV_Register(0)
    , m_UAV_Register(0)
{
}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& _Other)
    : CEntity(_Other)
    , m_SB_Main(nullptr)
    , m_SB_Write(nullptr)
    , m_SB_Read(nullptr)
    , m_Desc(_Other.m_Desc)
    , m_SRV(nullptr)
    , m_UAV(nullptr)
    , m_ElementSize(_Other.m_ElementSize)
    , m_ElementCount(_Other.m_ElementCount)
    , m_Type(_Other.m_Type)
    , m_bSysMemMove(_Other.m_bSysMemMove)
    , m_SRV_Register(0)
    , m_UAV_Register(0)
{
    Create(m_ElementSize, m_ElementCount, m_Type, m_bSysMemMove);

    if (nullptr != _Other.m_SB_Main)
    {
        CONTEXT->CopyResource(m_SB_Main.Get(), _Other.m_SB_Main.Get());
    }
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove, void* _InitialData)
{
    // 기존에 가리키던 버퍼 및 SRV 를 가리키지 않는다.
    m_SB_Main = nullptr;
    m_SB_Write = nullptr;
    m_SB_Read = nullptr;

    m_SRV = nullptr;
    m_UAV = nullptr;

    m_Type = _Type;
    m_bSysMemMove = _bSysMemMove;

    // 버퍼의 크기를 16의 배수로 설정해야한다.
    assert(!(_ElementSize % 16));

    m_ElementSize = _ElementSize;
    m_ElementCount = _ElementCount;

    m_Desc.ByteWidth = m_ElementSize * m_ElementCount;

    m_Desc.Usage = D3D11_USAGE_DEFAULT;
    m_Desc.CPUAccessFlags = 0;

    if (SB_TYPE::SRV_ONLY == m_Type)
    {
        m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    }
    else
    {
        m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    }

    m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    m_Desc.StructureByteStride = _ElementSize;

    HRESULT hr = E_FAIL;

    if (_InitialData)
    {
        D3D11_SUBRESOURCE_DATA tSub = {};
        tSub.pSysMem = _InitialData;
        hr = DEVICE->CreateBuffer(&m_Desc, &tSub, m_SB_Main.GetAddressOf());
    }    
    else
    {
        hr = DEVICE->CreateBuffer(&m_Desc, nullptr, m_SB_Main.GetAddressOf());
    }

    if (FAILED(hr))
        return E_FAIL;

    // ShaderResourceView 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC tViewDesc = {};
    tViewDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
    tViewDesc.BufferEx.NumElements = m_ElementCount;
    hr = DEVICE->CreateShaderResourceView(m_SB_Main.Get(), &tViewDesc, m_SRV.GetAddressOf());

    // Unordered AccessView 생성
    if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC tViewDesc = {};
        tViewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        tViewDesc.Buffer.NumElements = m_ElementCount;
        if (FAILED(DEVICE->CreateUnorderedAccessView(m_SB_Main.Get(), &tViewDesc, m_UAV.GetAddressOf())))
        {
            assert(nullptr);
            return E_FAIL;
        }
    }

    // 추가 버퍼 생성
    if (m_bSysMemMove)
    {
        // Write Buffer
        D3D11_BUFFER_DESC WriteDesc = {};
        WriteDesc = m_Desc;
        WriteDesc.Usage = D3D11_USAGE_DYNAMIC;
        WriteDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        WriteDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        if (FAILED(DEVICE->CreateBuffer(&WriteDesc, nullptr, m_SB_Write.GetAddressOf())))
        {
            assert(nullptr);
            return E_FAIL;
        }

        // Read Buffer
        D3D11_BUFFER_DESC ReadDesc = {};
        ReadDesc = m_Desc;
        ReadDesc.Usage = D3D11_USAGE_DEFAULT;
        ReadDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        ReadDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        if (FAILED(DEVICE->CreateBuffer(&ReadDesc, nullptr, m_SB_Read.GetAddressOf())))
        {
            assert(nullptr);
            return E_FAIL;
        }
    }

    return hr;
}

void CStructuredBuffer::SetData(void* _pData, UINT _ElementCount)
{
    if (!_ElementCount)
    {
        _ElementCount = m_ElementCount;
    }

    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);    
    memcpy(tSub.pData, _pData, _ElementCount * m_ElementSize);
    CONTEXT->Unmap(m_SB_Write.Get(), 0);

    // Write -> Main
    CONTEXT->CopyResource(m_SB_Main.Get(), m_SB_Write.Get());
}

void CStructuredBuffer::GetData(void* _pDest, UINT _ElementCount)
{
    if (!_ElementCount)
    {
        _ElementCount = m_ElementCount;
    }

    // Main -> Read
    CONTEXT->CopyResource(m_SB_Read.Get(), m_SB_Main.Get());

    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP_READ, 0, &tSub);
    memcpy(_pDest, tSub.pData, _ElementCount * m_ElementSize);   
    CONTEXT->Unmap(m_SB_Read.Get(), 0);
}

void CStructuredBuffer::ClearData()
{
    
}

void CStructuredBuffer::Binding(UINT _RegisterNum)
{    
    m_SRV_Register = _RegisterNum;
    CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::Binding_CS_SRV(UINT _RegisterNum)
{
    m_SRV_Register = _RegisterNum;
    CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::Binding_CS_UAV(UINT _RegisterNum)
{
    m_UAV_Register = _RegisterNum;

    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
}

void CStructuredBuffer::Clear_SRV()
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->VSSetShaderResources(m_SRV_Register, 1, &pSRV);
    CONTEXT->HSSetShaderResources(m_SRV_Register, 1, &pSRV);
    CONTEXT->DSSetShaderResources(m_SRV_Register, 1, &pSRV);
    CONTEXT->GSSetShaderResources(m_SRV_Register, 1, &pSRV);
    CONTEXT->PSSetShaderResources(m_SRV_Register, 1, &pSRV);
    CONTEXT->CSSetShaderResources(m_SRV_Register, 1, &pSRV);
}

void CStructuredBuffer::Clear_UAV()
{
    UINT i = -1;
    ID3D11UnorderedAccessView* pUAV = nullptr;
    CONTEXT->CSSetUnorderedAccessViews(m_UAV_Register, 1, &pUAV, &i);
}
