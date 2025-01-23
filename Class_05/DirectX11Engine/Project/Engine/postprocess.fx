#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

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

// mesh : RectMesh
VS_OUT VS_PostProcess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_PostProcess(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if(g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        vOutColor = (vOutColor.r + vOutColor.g + vOutColor.b) / 3.f;
    }
    
    return vOutColor;
}





// mesh : RectMesh
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_btex_0)
    {
        // 해당 픽셀의 픽셀 좌표 /  화면 전체 해상도 정보
        float2 vScreenUV = _in.vPosition.xy / vResolution;
        
        float2 vOffsetUV = float2(0.f, 0.f);
        if(g_btex_1)
        {
            _in.vUV.x += Time * 0.1f;
            
            vOffsetUV = g_tex_1.Sample(g_sam_0, _in.vUV);
            vOffsetUV -= 0.5f;
            vOffsetUV *= 0.02f;
        }                
        
        vOutColor = g_tex_0.Sample(g_sam_0, vScreenUV + vOffsetUV);
    }
    
    //vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return vOutColor;
}



#endif