#pragma once
#include "CAsset.h"


class CTexture :
    public CAsset
{
private:
    ScratchImage                        m_Image;
    ComPtr<ID3D11Texture2D>             m_Tex2D;

    ComPtr<ID3D11RenderTargetView>      m_RTV;
    ComPtr<ID3D11DepthStencilView>      m_DSV;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_TEXTURE2D_DESC                m_Desc;

public:
    float GetWidth() { return (float)m_Desc.Width; }
    float GetHeight() {return (float)m_Desc.Height; }

    ComPtr<ID3D11Texture2D> GetTex2D() { return m_Tex2D; }
    ComPtr<ID3D11RenderTargetView> GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView> GetDSV() { return m_DSV; }
    ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_SRV; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV() { return m_UAV; }

public:
    void Binding(int _RegisterNum);
    void Binding_CS_SRV(int _RegisterNum);
    void Binding_CS_UAV(int _RegisterNum);

    static void Clear(int _RegisterNum);
    static void Clear_CS_SRV(int _RegisterNum);
    static void Clear_CS_UAV(int _RegisterNum);



private:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;


public:
    // _BindFlag : D3D11_BIND_FLAG
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _BindFlag, D3D11_USAGE _Usage);
    int Create(ComPtr<ID3D11Texture2D> _Tex2D);

public:
    CLONE_DISABLE(CTexture);
public:
    CTexture(bool _bEngine = false);
    ~CTexture();

    friend class CAssetMgr;
};

