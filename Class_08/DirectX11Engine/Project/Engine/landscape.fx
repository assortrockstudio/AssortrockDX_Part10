#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "value.fx"
#include "func.fx"


// =============================
// LandScpae Shader
// MRT : Deferred
// DOMAIN : Deferred
// Mesh : 자체 Mesh 
// Parameter
#define FACE_X              g_int_0
#define FACE_Z              g_int_1
#define MODE                g_int_2
#define TEXTURE_ARRSIZE     g_int_3

#define MIN_LEVEL           g_vec4_0.x
#define MAX_LEVEL           g_vec4_0.y
#define MIN_RANGE           g_vec4_0.z
#define MAX_RANGE           g_vec4_0.w

#define CAM_POS             g_vec4_1.xyz

#define HEIGHT_MAP          g_tex_0
#define HasHeightMap        g_btex_0

#define HasColorTex         g_btexarr_0
#define HasNormalTex        g_btexarr_1

#define COLOR_TEX           g_texarr_0
#define NORMAL_TEX          g_texarr_1

#define IsShowBrush         g_btex_1
#define BRUSH_TEX           g_tex_1
#define BrushScale          g_vec2_0
#define BrushPos            g_vec2_1

StructuredBuffer<tWeight>   WEIGHT_MAP : register(t20);
#define WEIGHT_RESOLUTION   g_vec2_2

// =============================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;  
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float3 vWorldPos : POSITION1;
    float2 vUV : TEXCOORD;  
};

VS_OUT VS_LandScape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // 정점쉐이더는 어떠한 역할도 수행하지 않는다.
    // 입력값을 HullShader 로 그대로 넘긴다.
    output.vPos = _in.vPos;
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
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
        
    // 위, 아래
    Level.arrEdge[0] = GetTessFactor( MIN_LEVEL, MAX_LEVEL
                                    , MIN_RANGE, MAX_RANGE, CAM_POS
                                    , (_Patch[1].vWorldPos + _Patch[2].vWorldPos) * 0.5f);
    
    // 좌, 우
    Level.arrEdge[1] = GetTessFactor( MIN_LEVEL, MAX_LEVEL
                                    , MIN_RANGE, MAX_RANGE, CAM_POS
                                    , (_Patch[0].vWorldPos + _Patch[2].vWorldPos) * 0.5f);
    
    // 빗변
    Level.arrEdge[2] = GetTessFactor( MIN_LEVEL, MAX_LEVEL
                                    , MIN_RANGE, MAX_RANGE, CAM_POS
                                    , (_Patch[0].vWorldPos + _Patch[1].vWorldPos) * 0.5f);
    
    // 삼각형 중심
    Level.Inside     = GetTessFactor( MIN_LEVEL, MAX_LEVEL
                                    , MIN_RANGE, MAX_RANGE, CAM_POS
                                    , (_Patch[0].vWorldPos + _Patch[1].vWorldPos + _Patch[2].vWorldPos) / 3.f);
    
    return Level;
}

struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;   
};

// 정점 당 호출되는 함수
[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[maxtessfactor(64)]
[patchconstantfunc("PatchConstFunc")]
HS_OUT HS_LandScape(InputPatch<VS_OUT, 3> _Patch
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
    float2 vFullUV : TEXCOORD1;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

[domain("tri")]
DS_OUT DS_LandScape(   const OutputPatch<HS_OUT, 3> _Patch
                     , float3 _Weight : SV_DomainLocation
                     , TessLevel _TessLevel)
{
    DS_OUT output = (DS_OUT) 0.f;
    
    float3 vLocalPos = (float3) 0.f;
    float2 vUV = (float2) 0.f;
    
    float3 vTangent = float3(1.f, 0.f, 0.f);
    float3 vNormal = float3(0.f, 1.f, 0.f);
    float3 vBinormal = float3(0.f, 0.f, -1.f);
    
    for (int i = 0; i < 3; ++i)
    {
        vLocalPos += _Patch[i].vPos * _Weight[i];
        vUV += _Patch[i].vUV * _Weight[i];
    }
    
    // 지형 전체 기준 UV
    output.vFullUV = vUV / float2((float) FACE_X, (float) FACE_Z);
    
    // 높이맵 적용 
    if (HasHeightMap)
    {
        vLocalPos.y = g_tex_0.SampleLevel(g_sam_2, output.vFullUV, 0).x;
        
        // 노말벡터 재 계산
        // 주변 정점의 좌표를 알아낸다.        
        float fStep      = 1.f / _TessLevel.Inside;
        float2 vUVStep   = fStep / float2(FACE_X, FACE_Z);
                
        float2 vLeftUV   = output.vFullUV + float2(-vUVStep.x, 0.f);
        float2 vRightUV  = output.vFullUV + float2(vUVStep.x, 0.f);
        float2 vUpUV     = output.vFullUV + float2(0.f, -vUVStep.y);
        float2 vDownUV   = output.vFullUV + float2(0.f, vUVStep.y);
        
        float3 vLeftPos  = float3(vLocalPos.x - fStep, g_tex_0.SampleLevel(g_sam_2, vLeftUV , 0).x, vLocalPos.z);
        float3 vRightPos = float3(vLocalPos.x + fStep, g_tex_0.SampleLevel(g_sam_2, vRightUV, 0).x, vLocalPos.z);
        float3 vUpPos    = float3(vLocalPos.x        , g_tex_0.SampleLevel(g_sam_2, vUpUV   , 0).x, vLocalPos.z + fStep);
        float3 vDownPos  = float3(vLocalPos.x        , g_tex_0.SampleLevel(g_sam_2, vDownUV , 0).x, vLocalPos.z - fStep);               
                
        vLeftPos  = mul(float4(vLeftPos , 1.f), g_matWorld).xyz;
        vRightPos = mul(float4(vRightPos, 1.f), g_matWorld).xyz;
        vUpPos    = mul(float4(vUpPos   , 1.f), g_matWorld).xyz;
        vDownPos  = mul(float4(vDownPos , 1.f), g_matWorld).xyz;
                
        vTangent = normalize(vRightPos - vLeftPos);
        vBinormal = normalize(vDownPos - vUpPos);
        vNormal = normalize(cross(vTangent, vBinormal));
        
        output.vViewTangent = normalize(mul(float4(vTangent, 0.f), g_matView)).xyz;
        output.vViewBinormal = normalize(mul(float4(vBinormal, 0.f), g_matView)).xyz;
        output.vViewNormal = normalize(mul(float4(vNormal, 0.f), g_matView)).xyz;
    }
    else
    {
        output.vViewTangent = normalize(mul(float4(vTangent, 0.f), g_matWV)).xyz;
        output.vViewBinormal = normalize(mul(float4(vBinormal, 0.f), g_matWV)).xyz;
        output.vViewNormal = normalize(mul(float4(vNormal, 0.f), g_matWV)).xyz;
    }
    
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
        
    output.vViewPos = mul(float4(vLocalPos, 1.f), g_matWV).xyz;      
    
    return output;
}


struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float4 vEmissive : SV_Target3;
    float4 vData : SV_Target4;
};

PS_OUT PS_LandScape(DS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
   
    float4 vBrush = (float4) 0.f;
    
    if (IsShowBrush && MODE)
    {
        // Brush LeftTop 좌표
        float2 BrushLT = BrushPos - (BrushScale * 0.5f);
        
        // 지형 기준, 픽셀의 위치 구하기
        float2 vBrusUV = _in.vUV / float2(FACE_X, FACE_Z);
        vBrusUV = (vBrusUV - BrushLT) / BrushScale;
        
        if (0.f <= vBrusUV.x && vBrusUV.x <= 1.f
            && 0.f <= vBrusUV.y && vBrusUV.y <= 1.f)
        {
            float BrushAlpha = BRUSH_TEX.Sample(g_sam_2, vBrusUV).a;
            float3 BrushColor = float3(0.8f, 0.8f, 0.f);
            
            vBrush.rgb = (vBrush.rgb * (1 - BrushAlpha)) + (BrushColor * BrushAlpha);
        }
    }   
    
    float4 vColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    float3 vViewNormal = _in.vViewNormal;    
    
    if (HasColorTex)
    {
        //vColor = COLOR_TEX.Sample(g_sam_0, float3(_in.vUV, 6.f));
        float2 vColRow = _in.vFullUV * WEIGHT_RESOLUTION;
        int WeightIdx = WEIGHT_RESOLUTION.x * vColRow.y + vColRow.x;

        vColor = (float4) 0.f;
        
        int WeightMaxIdx = -1;
        float WeightMax = 0.f;
        
        for (int i = 0; i < TEXTURE_ARRSIZE; ++i)
        {
            float Weight = WEIGHT_MAP[WeightIdx].Weight[i];
            vColor += COLOR_TEX.Sample(g_sam_0, float3(_in.vUV, i)) * Weight;
            
            // 제일 높았던 가중치를 기록
            if (WeightMax < Weight)
            {
                WeightMax = Weight;
                WeightMaxIdx = i;
            }
        }
        
        if (WeightMaxIdx != -1)
        {
            float3 vNormal = NORMAL_TEX.Sample(g_sam_0, float3(_in.vUV, WeightMaxIdx));           
            vNormal = vNormal * 2.f - 1.f;
        
            float3x3 Rot =
            {
                _in.vViewTangent,
                _in.vViewBinormal,
                _in.vViewNormal
            };
        
            vViewNormal = normalize(mul(vNormal, Rot));
        }
    }
    
    output.vColor = vColor;    
    output.vEmissive = float4(vBrush.rgb, 1.f);
    output.vNormal = float4(vViewNormal, 1.f);
    output.vPosition = float4(_in.vViewPos, 1.f);    
    
    return output;
}

#endif