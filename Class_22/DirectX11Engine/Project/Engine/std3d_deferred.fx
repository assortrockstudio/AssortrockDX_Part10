#ifndef _STD3D_DEFERRED
#define _STD3D_DEFERRED

#include "value.fx"
#include "func.fx"

// =====================
// Std3D_Deferred Shader
// MRT		: Deferred
// g_tex_0	: Color Texture
// g_tex_1	: Normal Texture
// g_tex_2  : Specular Texture
// g_tex_3  : Emissive Texture
// ========================
struct VS_IN
{
	float3 vPos		: POSITION;
	float2 vUV		: TEXCOORD;
		
    float3 vTangent : TANGENT;
    float3 vNormal	: NORMAL;
    float3 vBinormal: BINORMAL;
    
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_OUT
{
	float4 vPosition	: SV_Position;
	float2 vUV			: TEXCOOR;
	
    float3 vViewPos		: POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewNormal	: NORMAL;
    float3 vViewBinormal: BINORMAL;
};


VS_OUT VS_Std3D_Deferred(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;
	
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal
              , _in.vWeights, _in.vIndices, 0);
    }
    
	output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
	output.vUV = _in.vUV;
	
    output.vViewPos		= mul(float4(_in.vPos, 1.f), g_matWV);    
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewBinormal= normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));
    output.vViewNormal	= normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
	
	return output;
}


struct VS_IN_Inst
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
		
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
    
    // Per Instance Data    
    row_major matrix matWorld   : WORLD;
    row_major matrix matWV      : WV;
    row_major matrix matWVP     : WVP;
    uint iRowIndex              : ROWINDEX; // 자신의 애니메이션 최종 행렬 데이터가 몇번째 행에 있는지
};

VS_OUT VS_Std3D_Deferred_Inst(VS_IN_Inst _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal
              , _in.vWeights, _in.vIndices, _in.iRowIndex);
    }
    
    output.vPosition = mul(float4(_in.vPos, 1.f), _in.matWVP);
    output.vUV       = _in.vUV;
	
    output.vViewPos         = mul(float4(_in.vPos, 1.f), _in.matWV);
    output.vViewTangent     = normalize(mul(float4(_in.vTangent, 0.f), _in.matWV));
    output.vViewBinormal    = normalize(mul(float4(_in.vBinormal, 0.f), _in.matWV));
    output.vViewNormal      = normalize(mul(float4(_in.vNormal, 0.f), _in.matWV));
	
    return output;
}


struct PS_OUT
{
	float4 vColor		: SV_Target0;
	float4 vNormal		: SV_Target1;
	float4 vPosition	: SV_Target2;
	float4 vEmissive	: SV_Target3;
	float4 vData		: SV_Target4;
};


PS_OUT PS_Std3D_Deferred(VS_OUT _in)
{
	PS_OUT output = (PS_OUT) 0.f;
		
    float4 vObjectColor = float4(1.f, 0.f, 1.f, 1.f);	
	if(g_btex_0)
    {
        vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV) * MtrlData.vDiff;
    }
		
    float3 vViewNormal = _in.vViewNormal;
	if(g_btex_1)
    {
		// Sample 로 얻은 값은 0 ~ 1 범위, 실제 기록된 값의 의미는 -1 ~ 1
        float3 vNormal = g_tex_1.Sample(g_sam_0, _in.vUV);
        vNormal = vNormal * 2.f - 1.f;
        
        float3x3 Rot =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal
        };
        
        vViewNormal = normalize(mul(vNormal, Rot));
    }
	
    float4 vEmissie = (float4) 0.f;
    if(g_btex_3)
    {
        vEmissie = g_tex_3.Sample(g_sam_0, _in.vUV) * MtrlData.vEmv;
    }        
    
	output.vColor	 = float4(vObjectColor.xyz, 1.f);
	output.vNormal	 = float4(vViewNormal, 1.f);
    output.vPosition = float4(_in.vViewPos, 1.f);
    output.vEmissive = vEmissie;
    
	return output;
}

#endif