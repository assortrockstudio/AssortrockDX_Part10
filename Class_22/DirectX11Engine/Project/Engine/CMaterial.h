#pragma once
#include "CAsset.h"

#include "assets.h"

class CMaterial :
    public CAsset
{
private:    
    Ptr<CGraphicShader>     m_Shader;

    tMtrlConst              m_Const;
    Ptr<CTexture>           m_arrTex[TEX_PARAM::END];

    bool                    m_bDynamic;


public:
    void SetShader(Ptr<CGraphicShader> _Shader);
    Ptr<CGraphicShader> GetShader() { return m_Shader; }

    template<typename T>
    void SetScalarParam(SCALAR_PARAM _Param, const T& _Value);
    void SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _Tex);

    void SetMaterialCoefficient(Vec4 _vDiff, Vec4 _vSpec, Vec4 _vAmb, Vec4 _vEmis)
    {
        m_Const.mtrl.vDiff = _vDiff;
        m_Const.mtrl.vAmb = _vAmb;
        m_Const.mtrl.vSpec = _vSpec;
        m_Const.mtrl.vEmv = _vEmis;
    }

    void SetAnim3D(bool _bTrue) { m_Const.arrAnimData[0] = (int)_bTrue; }
    void SetBoneCount(int _iBoneCount) { m_Const.arrAnimData[1] = _iBoneCount; }

    void* GetScalarParam(SCALAR_PARAM _Param);
    Ptr<CTexture>& GetTexParam(TEX_PARAM _TexParam) {  return m_arrTex[_TexParam]; }

    bool IsDynamic() { return m_bDynamic; }

    Ptr<CMaterial> GetDynamicMaterial();


    void operator = (const CMaterial& _OtherMtrl)
    {
        SetName(_OtherMtrl.GetName());

        m_Const = _OtherMtrl.m_Const;

        for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
        {
            m_arrTex[i] = _OtherMtrl.m_arrTex[i];
        }

        m_Shader = _OtherMtrl.m_Shader;
    }

public:
    void Binding();
    void Binding_Inst();

    virtual int Load(const wstring& _FilePath);
    virtual int Save(const wstring& _FilePath);


public:
    CLONE(CMaterial);
    CMaterial(bool _bEngine = false);
    ~CMaterial();
};

template<typename T>
inline void CMaterial::SetScalarParam(SCALAR_PARAM _Param, const T& _Value)
{
    void* pValue = (void*)&_Value;

    switch (_Param)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
        m_Const.iArr[_Param - INT_0] = *((int*)pValue);
        break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
        m_Const.fArr[_Param - FLOAT_0] = *((float*)pValue);
        break;
    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
        m_Const.v2Arr[_Param - VEC2_0] = *((Vec2*)pValue);
        break;
    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
        m_Const.v4Arr[_Param - VEC4_0] = *((Vec4*)pValue);
        break;
    case MAT_0:
    case MAT_1:
    case MAT_2:
    case MAT_3:
        m_Const.matArr[_Param - MAT_0] = *((Matrix*)pValue);
        break;
    }
}