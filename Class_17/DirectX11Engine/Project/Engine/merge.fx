#ifndef _MERGE
#define _MERGE

#include "value.fx"


#include "value.fx"
#include "func.fx"

// ======================
// MergeShader
// Domain : DOMAIN_LIGHTING
// MRT    : SWAPCHAIN
// Mesh   : RectMesh
#define ColorTarget     g_tex_0
#define DiffuseTarget   g_tex_1
#define SpecularTarget  g_tex_2
#define EmissiveTarget  g_tex_3
#define PositionTarget  g_tex_4
#define RenderTarget    g_tex_5

#define OutputTarget    g_int_0         
// ======================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Merge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // Directional Light 는 화면의 모든 픽셀에 대해서 픽셀쉐이더가 호출되도록 함
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Merge(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
              
    if (OutputTarget)
    {
        vOutColor = RenderTarget.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        float4 vDiffuse = DiffuseTarget.Sample(g_sam_0, _in.vUV);
        float4 vSpecular = SpecularTarget.Sample(g_sam_0, _in.vUV);
        float4 vEmissive = EmissiveTarget.Sample(g_sam_0, _in.vUV);        
        float4 vColor = ColorTarget.Sample(g_sam_0, _in.vUV);
        
        vOutColor.rgb = vColor.rgb * (vDiffuse.rgb) + vSpecular.rgb + vEmissive.rgb;
    }
    
    return vOutColor;
}




#endif