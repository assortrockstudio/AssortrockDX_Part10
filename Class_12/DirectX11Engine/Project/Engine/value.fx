#ifndef _VALUE
#define _VALUE

#include "struct.fx"


cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matWorldInv;
    
    row_major matrix g_matView;
    row_major matrix g_matViewInv;
    row_major matrix g_matProj;
    row_major matrix g_matProjInv;    
    
    row_major matrix g_matWV;    
    row_major matrix g_matWVP;
};

cbuffer MATERIAL_CONST : register(b1)
{
    int                     g_int_0;
    int                     g_int_1;
    int                     g_int_2;
    int                     g_int_3;
    
    float                   g_float_0;
    float                   g_float_1;
    float                   g_float_2;
    float                   g_float_3;    
    
    float2                  g_vec2_0;
    float2                  g_vec2_1;
    float2                  g_vec2_2;
    float2                  g_vec2_3;
    
    float4                  g_vec4_0;
    float4                  g_vec4_1;
    float4                  g_vec4_2;
    float4                  g_vec4_3;
    
    row_major matrix        g_mat_0;
    row_major matrix        g_mat_1;
    row_major matrix        g_mat_2;
    row_major matrix        g_mat_3;
    
    uint                    g_btex_0;
    uint                    g_btex_1;
    uint                    g_btex_2;
    uint                    g_btex_3;
    uint                    g_btex_4;
    uint                    g_btex_5;
    uint                    g_btexcube_0;
    uint                    g_btexcube_1;
    uint                    g_btexcube_2;
    uint                    g_btexcube_3;
    uint                    g_btexarr_0;
    uint                    g_btexarr_1;
    uint                    g_btexarr_2;
    uint                    g_btexarr_3;
}

cbuffer ANIMATION2D : register(b2)
{
    float2 vLeftTop;
    float2 vSliceSize;
    float2 vOffset;
    float2 vBackground;
    int UseAnim2D;
    int3 padding;
}

cbuffer GLOBAL_DATA : register(b3)
{
    float2  vResolution;    // 렌더링 해상도
    
    float   DT;             // DT
    float   Time;           // 게임 시작한후 지난 시간

    float   DT_Engine;      // DT
    float   Time_Engine;    // 게임 시작한후 지난 시간
        
    int     Light2DCount;
    int     Light3DCount;
}

Texture2D       g_tex_0 : register(t0);
Texture2D       g_tex_1 : register(t1);
Texture2D       g_tex_2 : register(t2);
Texture2D       g_tex_3 : register(t3);
Texture2D       g_tex_4 : register(t4);
Texture2D       g_tex_5 : register(t5);

TextureCube     g_texcube_0 : register(t6);
TextureCube     g_texcube_1 : register(t7);
TextureCube     g_texcube_2 : register(t8);
TextureCube     g_texcube_4 : register(t9);

Texture2DArray  g_texarr_0 : register(t10);
Texture2DArray  g_texarr_1 : register(t11);
Texture2DArray  g_texarr_2 : register(t12);
Texture2DArray  g_texarr_4 : register(t13);

// Animation Atlas 바인딩
Texture2D g_Atlas : register(t14);

// Light2DInfo
StructuredBuffer<tLightInfo> g_Light2D : register(t15);

// Light3DInfo
StructuredBuffer<tLightInfo> g_Light3D : register(t16);



SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);
SamplerState g_sam_2 : register(s2);

#define PI 3.1415926535f


#endif