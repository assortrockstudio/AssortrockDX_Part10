#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture(bool _bEngine)
    : CAsset(ASSET_TYPE::TEXTURE, _bEngine)
    , m_Desc{}
{
}

CTexture::~CTexture()
{
}

void CTexture::Binding(int _RegisterNum)
{
    CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());    
}


void CTexture::Binding_CS_SRV(int _RegisterNum)
{
    CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Binding_CS_UAV(int _RegisterNum)
{
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
}

void CTexture::Clear(int _RegisterNum)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
    CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
    CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
    CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
    CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}

void CTexture::Clear_CS_SRV(int _RegisterNum)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->CSSetShaderResources(_RegisterNum, 1, &pSRV);
}

void CTexture::Clear_CS_UAV(int _RegisterNum)
{
    ID3D11UnorderedAccessView* pUAV = nullptr;
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, &pUAV, &i);
}

int CTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat
                    , UINT _BindFlag, D3D11_USAGE _Usage)
{       
    m_Desc.Format = _PixelFormat;
    m_Desc.Width = _Width;
    m_Desc.Height = _Height;
    m_Desc.ArraySize = 1;

    m_Desc.Usage = _Usage;
    if (D3D11_USAGE_DYNAMIC == m_Desc.Usage)
    {
        m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }    
    
    m_Desc.BindFlags = _BindFlag;
    m_Desc.MipLevels = 1;
    m_Desc.SampleDesc.Count = 1;
    m_Desc.SampleDesc.Quality = 0;

    if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, &m_Tex2D)))
    {
        return E_FAIL;
    }

    if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
    {
        DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
    }

    else
    {
        if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
        {
            DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
        }

        if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC Desc = {};
            Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            Desc.Texture2D.MipLevels = 1;
            Desc.Texture2D.MostDetailedMip = 0;
            DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &Desc, m_SRV.GetAddressOf());
        }

        if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC Desc = {};
            Desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
            DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), &Desc, m_UAV.GetAddressOf());
        }
    }     

    return S_OK;
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _Tex2D)
{
    m_Tex2D = _Tex2D;
    m_Tex2D->GetDesc(&m_Desc);

    if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
    {
        DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
    }

    else
    {
        if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
        {
            DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
        }

        if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC Desc = {};
            Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            Desc.Texture2D.MipLevels = 1;
            Desc.Texture2D.MostDetailedMip = 0;
            DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &Desc, m_SRV.GetAddressOf());
        }

        if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC Desc = {};
            Desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
            DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), &Desc, m_UAV.GetAddressOf());
        }
    }

    return S_OK;
}


int CTexture::Load(const wstring& _FilePath)
{
    wchar_t Ext[50] = {};
    _wsplitpath_s(_FilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, Ext, 50);

    wstring strExt = Ext;

    HRESULT hr = E_FAIL;

    if (strExt == L".dds" || strExt == L".DDS")
    {
        // .dds .DDS
        hr = LoadFromDDSFile(_FilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
    }
    else if (strExt == L".tga" || strExt == L".TGA")
    {
        // .tag .TGA
        hr = LoadFromTGAFile(_FilePath.c_str(), nullptr, m_Image);
    }
    else
    {
        // .png .jpg .jpeg .bmp
        hr = LoadFromWICFile(_FilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"텍스쳐 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
        return E_FAIL;
    }


    // Texture2D Description 작성해서 Texture2D 객체 생성
    // Texture2D 객체를 이용해서 ShaderResourceView 생성
    hr = CreateShaderResourceView( DEVICE
                            , m_Image.GetImages()
                            , m_Image.GetImageCount()
                            , m_Image.GetMetadata()
                            , m_SRV.GetAddressOf());
    if (FAILED(hr))
    {
        MessageBox(nullptr, L"ShaderResourceView 생성 실패", L"텍스쳐 로딩 실패", MB_OK);
        return E_FAIL;
    }

    m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());
    m_Tex2D->GetDesc(&m_Desc);

    return hr;
}

int CTexture::Save(const wstring& _FilePath)
{
    return 0;
}



