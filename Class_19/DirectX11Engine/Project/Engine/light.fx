#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "func.fx"

// ======================
// DirLightShader
// Domain : DOMAIN_LIGHTING
// MRT    : LIGHT
// Mesh   : RectMesh
#define LightIdx        g_int_0
#define PositionTarget  g_tex_0
#define NormalTarget    g_tex_1

#define SHADOW_MAP      g_tex_2
#define LIGHT_VP        g_mat_0
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

VS_OUT VS_DirLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // Directional Light 는 화면의 모든 픽셀에 대해서 픽셀쉐이더가 호출되도록 함
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

struct PS_OUT
{
    float4 vDiffuse     : SV_Target;
    float4 vSpecular    : SV_Target1;    
};

PS_OUT PS_DirLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
        
    // 호출된 픽셀과 동일한 위치에 기록된 Position 값을 가져온다.
    float4 vPosition = PositionTarget.Sample(g_sam_0, _in.vUV);
    
    // Position 값이 존재하지 않으면, 빛을 받을 물체가 존재하지 않는다.
    if (0.f == vPosition.a)
    {
        discard;
    }
        
    // 빛을 받을 위치를 월드좌표계 기준으로 변경한다.
    float3 vWorldPos = mul(float4(vPosition.xyz, 1.f), g_matViewInv);    
    float4 vLightProjPos = mul(float4(vWorldPos, 1.f), LIGHT_VP);    
    float2 vShadowMapUV = vLightProjPos.xy / vLightProjPos.w;
    
    vShadowMapUV.x = vShadowMapUV.x / 2.f + 0.5f;
    vShadowMapUV.y = 1.f - (vShadowMapUV.y / 2.f + 0.5f);
    
    float Depth = vLightProjPos.z / vLightProjPos.w;
        
    // 그림자 판정
    float fShadowPow = 0.f;
    float fShadowDepth = SHADOW_MAP.Sample(g_sam_0, vShadowMapUV);
    
    // 광원쪽에서 기록한 깊이가 더 작다. 
    // 광원의 시야에서 해당 지점까지 가는 도중에 더 가까운 무언가가 광원 시야에 잡혔다.
    if (0 <= vShadowMapUV.x && vShadowMapUV.x <= 1.f
        && 0 <= vShadowMapUV.y && vShadowMapUV.y <= 1.f)
    {
        if (fShadowDepth + 0.001f < Depth)
        {
            // 그림자
            fShadowPow = 0.9f;
        }
    }
    
    // 빛 계산
    float4 vNormal = NormalTarget.Sample(g_sam_0, _in.vUV);
    
    tLight light = (tLight) 0.f;
    
    CalLight3D(LightIdx, vPosition.xyz, vNormal.xyz, light);
        
    output.vDiffuse = (light.vDiffuse + light.vAmbient) * (1.f - fShadowPow);
    output.vSpecular = light.vMaxSpecular * (1.f - fShadowPow);
        
    output.vDiffuse.a = vPosition.z;
    output.vSpecular.a = vPosition.z;
    
    return output;
}

// ==================================
// PointLightShader
// Domain           : DOMAIN_LIGHTING
// MRT              : LIGHT
// Mesh             : SphereMesh, Radius 0.5f
// DS_TYPE          : NO_TEST_NO_WRITE
// LightIdx           g_int_0
// PositionTarget     g_tex_0
// NormalTarget       g_tex_1
// ==================================
VS_OUT VS_PointLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // PointLight 는 VolumeMesh 를 실제 광원 위치에 배치함
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

PS_OUT PS_PointLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // 호출된 픽셀 쉐이더의 픽셀 좌표 / 렌더타겟 해상도
    float2 vScreenUV = _in.vPosition.xy / vResolution;
    
    // 호출된 픽셀과 동일한 위치에 기록된 Position 값을 가져온다.
    float4 vPosition = PositionTarget.Sample(g_sam_0, vScreenUV);
    
    // Position 값이 존재하지 않으면, 빛을 받을 물체가 존재하지 않는다.
    if (0.f == vPosition.a)
    {       
        discard;
    }
    
    // 기록되어있는 Position 값을 VolumeMesh 의 로컬 공간으로 이동시킨다.
    float4 vLocalPos = mul(mul(float4(vPosition.xyz, 1.f), g_matViewInv), g_matWorldInv);
    
    if (0.5f < length(vLocalPos.xyz))
    {     
        discard;
    }
            
    float4 vNormal = NormalTarget.Sample(g_sam_0, vScreenUV);
    
    tLight light = (tLight) 0.f;    
    CalLight3D(LightIdx, vPosition.xyz, vNormal.xyz, light);
        
    output.vDiffuse = light.vDiffuse + light.vAmbient;
    output.vSpecular = light.vMaxSpecular;
        
    output.vDiffuse.a = vPosition.z;
    output.vSpecular.a = vPosition.z;
    
    return output;
}




#endif