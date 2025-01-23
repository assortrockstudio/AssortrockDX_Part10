#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"
#include "CPathMgr.h"

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

int CTexture::CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex)
{
    m_Desc = _vecTex[0]->GetDesc();
    m_Desc.ArraySize = (UINT)_vecTex.size();
    m_Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;    
    m_Desc.MipLevels = 1;

    if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
    {
        return E_FAIL;
    }

    // 원본 각 텍스쳐를 생성된 배열 텍스쳐의 각 칸으로 복사시킨다.
    for (size_t i = 0; i < _vecTex.size(); ++i)
    {
        UINT Offset = D3D11CalcSubresource(0, i, 1);

        CONTEXT->UpdateSubresource(m_Tex2D.Get(), Offset, nullptr
                                  , _vecTex[i]->GetSysMem()
                                  , _vecTex[i]->GetRowPitch()
                                  , _vecTex[i]->GetSlicePitch());
    }

    // Shader Resrouce View 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};

    tSRVDesc.Format                         = m_Desc.Format;
    tSRVDesc.ViewDimension                  = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    tSRVDesc.Texture2DArray.MipLevels       = 1;
    tSRVDesc.Texture2DArray.MostDetailedMip = 0;
    tSRVDesc.Texture2DArray.ArraySize       = m_Desc.ArraySize;

    if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &tSRVDesc, m_SRV.GetAddressOf())))
        return E_FAIL;


    return S_OK;
}

int CTexture::GenerateMip(UINT _Level)
{
    // CubeTexture 는 Mipmap 생성 금지
    assert(false == m_Desc.MiscFlags & D3D11_SRV_DIMENSION_TEXTURECUBE);

    m_Tex2D = nullptr;
    m_RTV = nullptr;
    m_DSV = nullptr;
    m_SRV = nullptr;
    m_UAV = nullptr;

    m_Desc.MipLevels = _Level;
    m_Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    m_Desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

    if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
    {
        return E_FAIL;
    }

    for (UINT i = 0; i < m_Desc.ArraySize; ++i)
    {
        UINT iSubIdx = D3D11CalcSubresource(0, i, m_Desc.MipLevels);

        CONTEXT->UpdateSubresource(m_Tex2D.Get(), iSubIdx, nullptr
                                , m_Image.GetImage(0, i, 0)->pixels
                                , m_Image.GetImage(0, i, 0)->rowPitch
                                , m_Image.GetImage(0, i, 0)->slicePitch);
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
      
    if (2 <= m_Desc.ArraySize)
    {
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
        SRVDesc.Texture2DArray.ArraySize = m_Desc.ArraySize;
        SRVDesc.Texture2DArray.MipLevels = m_Desc.MipLevels;
        SRVDesc.Texture2DArray.MostDetailedMip = 0;
    }
    else
    {
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MipLevels = m_Desc.MipLevels;
        SRVDesc.Texture2D.MostDetailedMip = 0;
    }

    if(FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &SRVDesc, m_SRV.GetAddressOf())))
        return E_FAIL;

    CONTEXT->GenerateMips(m_SRV.Get());

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
    // GPU -> System
    CaptureTexture(DEVICE, CONTEXT, m_Tex2D.Get(), m_Image);

    // System -> File
    wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(_FilePath);
    SetRelativePath(strRelativePath);


    HRESULT hr = E_FAIL;
    if (1 == m_Image.GetMetadata().arraySize)
    {
        // png, jpg, jpeg, bmp, 
        hr = SaveToWICFile( *m_Image.GetImages()
                    , WIC_FLAGS_NONE
                    , GetWICCodec(WICCodecs::WIC_CODEC_PNG)
                    , _FilePath.c_str());
    }
    
    else
    {
        hr = SaveToDDSFile( m_Image.GetImages()
                     , m_Image.GetMetadata().arraySize
                     , m_Image.GetMetadata()
                     , DDS_FLAGS_NONE
                     , _FilePath.c_str());
    }

    return hr;
}



