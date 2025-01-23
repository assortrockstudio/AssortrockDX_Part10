#ifndef _DEBUG_SHAPE
#define _DEBUG_SHAPE

#include "value.fx"


struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;    
    
    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;
};


VS_OUT VS_DebugShape(float3 vLocalPos : POSITION, float2 _vUV : TEXCOORD, float3 vNormal : NORMAL)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(mul(mul(float4(vLocalPos, 1.f), g_matWorld), g_matView), g_matProj);
    output.vUV = _vUV;
    
    output.vViewPos = mul(float4(vLocalPos, 1.f), g_matWV);
    output.vViewNormal = normalize(mul(float4(vNormal, 0.f), g_matWV));       
        
    return output;
}

float4 PS_DebugShape(VS_OUT _in) : SV_Target
{  
    // 사용하는 메쉬가 SphereMesh 라면
    float Alpha = 1.f;
    
    if (4 == g_int_0)
    {
        float3 vEye = normalize(_in.vViewPos);
        Alpha = 1.f - saturate(dot(vEye, _in.vViewNormal));        
        Alpha = pow(Alpha, 2.f);
    }
    
    float4 vOutColor = g_vec4_0;
    vOutColor.a = Alpha;
    
    return vOutColor;
}



#endif