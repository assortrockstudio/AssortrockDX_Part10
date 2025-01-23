#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

StructuredBuffer<float4> g_TestBuffer : register(t20);


struct VS_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    
    float3 vWorldPos : POSITION;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // 행렬을 곱할때 3차원 좌표를 4차원으로 확장(동차좌표)    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}


// Rasterizer


float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
   
    if (UseAnim2D)
    {
        float2 vBkgroundLT = vLeftTop - (vBackground - vSliceSize) * 0.5f;
        float2 vUV = (vBkgroundLT + _in.vUV * vBackground) - vOffset;
        
        if (vUV.x < vLeftTop.x || vLeftTop.x + vSliceSize.x < vUV.x
            || vUV.y < vLeftTop.y || vLeftTop.y + vSliceSize.y < vUV.y)
        {
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_0, vUV);
        }
    }
    else
    {
        if(g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = float4(1.f, 0.f, 1.f, 1.f);
    }
        
    float3 vLightPow = 0.f;
    for (int i = 0; i < Light2DCount; ++i)
    {
        vLightPow += CalLight2D(i, _in.vWorldPos);
    }
    
    vColor.rgb *= vLightPow;
                
    if (g_btex_1)
    {
        float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);
                
        float fIntence = g_float_0 + vNoise.r;
                
        vColor.r += pow(fIntence * 10.f, 5) / pow(10, 5);
        vColor.g += pow(fIntence * 10.f, 5) / pow(10, 5) * 0.5f;
        vColor.b += pow(fIntence * 10.f, 5) / pow(10, 5) * 0.5f;
        
        if (1.f <= fIntence)
        {
            discard;
        }
    }   
    
    
    if(vColor.a == 0.f)
        discard;
    
    if(g_int_0)
        vColor.r *= 1.5f;
    
    return vColor;
}



float4 PS_Std2D_AB(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
   
    if (UseAnim2D)
    {
        float2 vBkgroundLT = vLeftTop - (vBackground - vSliceSize) * 0.5f;
        float2 vUV = (vBkgroundLT + _in.vUV * vBackground) - vOffset;
        
        if (vUV.x < vLeftTop.x || vLeftTop.x + vSliceSize.x < vUV.x
            || vUV.y < vLeftTop.y || vLeftTop.y + vSliceSize.y < vUV.y)
        {            
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_0, vUV);
        }
    }
    else
    {
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
        
    float3 vLightPow = 0.f;
    for (int i = 0; i < Light2DCount; ++i)
    {
        vLightPow += CalLight2D(i, _in.vWorldPos);
    }
    
    vColor.rgb *= vLightPow;
    
    return vColor;
}

#endif