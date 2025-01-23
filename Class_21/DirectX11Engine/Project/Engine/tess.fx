#ifndef _TESS
#define _TESS

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;   
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float3 vPos : POSITION;   
    float2 vUV  : TEXCOORD;    
};

VS_OUT VS_Tess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // 정점쉐이더는 어떠한 역할도 수행하지 않는다.
    // 입력값을 HullShader 로 그대로 넘긴다.
    output.vPos = _in.vPos;
    output.vUV  = _in.vUV;
    
    return output;
}

// ===========
// Hull Shader
// ===========
struct TessLevel
{
    float arrEdge[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

// 패치상수함수 - 패치 당 호출되는 함수
TessLevel PatchConstFunc(InputPatch<VS_OUT, 3> _Patch, uint _PatchID : SV_PrimitiveID)
{
    TessLevel Level = (TessLevel) 0.f;
        
    Level.arrEdge[0] = 32.f;
    Level.arrEdge[1] = 32.f;
    Level.arrEdge[2] = 32.f;
    Level.Inside = 32.f;
    
    return Level;
}

struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

// 정점 당 호출되는 함수
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[maxtessfactor(64)]
[patchconstantfunc("PatchConstFunc")]
HS_OUT HS_Tess( InputPatch<VS_OUT, 3> _Patch
                , uint _VtxIdx : SV_OutputControlPointID
                , uint _PatchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT) 0.f;    
    
    output.vPos = _Patch[_VtxIdx].vPos;
    output.vUV = _Patch[_VtxIdx].vUV;    
    
    return output;
}


// =============
// Domain Shader
// =============
struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

[domain("tri")]
DS_OUT DS_Tess(const OutputPatch<HS_OUT, 3> _Patch
             , float3 _Weight : SV_DomainLocation
             , TessLevel _TessLevel)
{
    DS_OUT output = (DS_OUT) 0.f;
    
    float3 vLocalPos =   _Patch[0].vPos * _Weight[0]
                       + _Patch[1].vPos * _Weight[1]
                       + _Patch[2].vPos * _Weight[2];
    
    float2 vUV = _Patch[0].vUV * _Weight[0]
                 + _Patch[1].vUV * _Weight[1]
                 + _Patch[2].vUV * _Weight[2];    
    
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    
    return output;
}


float4 PS_Tess(DS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    return vColor;
}

#endif