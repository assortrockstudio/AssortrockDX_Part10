#ifndef _SKYBOX
#define _SKYBOX

#include "value.fx"
#include "func.fx"

#define SKYBOX_TYPE g_int_0

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;    
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;    
    float3 vCubeUV : TEXCOORD1;    
};

VS_OUT VS_SkyBox(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // SphereMesh ÀÇ Local ÁÂÇ¥       
    float4 vViewPos = mul(float4(_in.vPos, 0.f), g_matView);        
    output.vPosition = mul(vViewPos, g_matProj);    
    output.vPosition.z = output.vPosition.w;
    
    output.vUV = _in.vUV;
    output.vCubeUV = normalize(_in.vPos);
    
    return output;
}

float4 PS_SkyBox(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // Sphere
    if (0 == SKYBOX_TYPE)
    {
        if (g_btex_0)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
    }
    
    // Cube
    else if (1 == SKYBOX_TYPE)
    {
        if (g_btexcube_0)
        {
            vOutColor = g_texcube_0.Sample(g_sam_0, _in.vCubeUV);
        }
    }
    
    return vOutColor;
}


#endif