#pragma once
#include "CAsset.h"

struct tShaderScalarParam
{
    string          strDesc;
    SCALAR_PARAM    Param;
};

struct tShaderTexParam
{
    string      strDesc;
    TEX_PARAM   Param;
};



class CShader :
    public CAsset
{
protected:    
    ComPtr<ID3DBlob>            m_ErrBlob;
    vector<tShaderScalarParam>  m_vecScalar;
    vector<tShaderTexParam>     m_vecTex;

public:
    void AddScalarParam(const string& _strDesc, SCALAR_PARAM _Param) { m_vecScalar.push_back(tShaderScalarParam{ _strDesc, _Param }); }
    void AddTexParam(const string& _strDesc, TEX_PARAM _Param) { m_vecTex.push_back(tShaderTexParam{ _strDesc, _Param }); }

    const vector<tShaderScalarParam>& GetScalarParam() { return m_vecScalar; }
    const vector<tShaderTexParam>&    GetTexParam(){ return m_vecTex; }

public:
    virtual int Binding() = 0;

    virtual int Load(const wstring& _FilePath) override { return S_OK; }
    virtual int Save(const wstring& _FilePath) override { return S_OK; }

    CLONE_DISABLE(CShader);
public:
    CShader(ASSET_TYPE _Type);
    ~CShader();
};

