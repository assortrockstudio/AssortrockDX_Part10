#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "func.fx"

StructuredBuffer<tParticle>         ParticleBuffer : register(t17);
StructuredBuffer<tParticleModule>   ModuleBuffer : register(t18);

#define Particle    ParticleBuffer[_in[0].InstID]
#define Module      ModuleBuffer[0]



struct VS_PARTICLE_IN
{
    float3 vPos     : POSITION;
    float2 vUV      : TEXCOORD;
    uint InstID     : SV_InstanceID;
};

struct VS_PARTICLE_OUT
{
    float3 vPos : POSITION;
    float2 vUV  : TEXCOORD;
    uint InstID : FOG;
};

// 1. VS 가 딱히 하는 일이 없어졌지만, 파이프라인 단계에서 VS 없이 호출될 수 없으므로,
//    GS 로 단순히 데이터 전달하는 기능이라도 구현해서 세팅해줘야 함.
VS_PARTICLE_OUT VS_Particle(VS_PARTICLE_IN _in)
{
    VS_PARTICLE_OUT output = (VS_PARTICLE_OUT) 0.f;
            
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;    
    output.InstID = _in.InstID;
    
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

[maxvertexcount(12)]
void GS_Particle(point VS_PARTICLE_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f};
    GS_OUT output_cross[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    // 1. 비활성화된 파티클의 경우, GS 단계에서 파이프라인을 중단시킬 수 있다.
    
    // 2. Point Mesh 를 사용해서 RectMesh 로 분할 할 예정, 중심점에 해당하는 Point Mesh 를
    //    ViewSpace 에서 확장시켜서, 자연스럽게 Billboard 처리가 되도록 함
        
    if (0 == Particle.Active)
    {
        return;
    }
    
    
    // 파티클 중심위치를 Local(원점) -> World -> View
    float4 vViewPos = mul(float4(Particle.vWorldPos, 1.f), g_matView);
   
    // ViewSpace 에서 4개로 분할
    // 0 -- 1
    // | \  |
    // 3 -- 2    
    output[0].vPosition = float4(-Particle.vWorldCurrentScale.x * 0.5f, Particle.vWorldCurrentScale.y * 0.5, 0.f, 0.f);
    output[1].vPosition = float4(Particle.vWorldCurrentScale.x * 0.5f, Particle.vWorldCurrentScale.y * 0.5, 0.f, 0.f);
    output[2].vPosition = float4(Particle.vWorldCurrentScale.x * 0.5f, -Particle.vWorldCurrentScale.y * 0.5, 0.f, 0.f);
    output[3].vPosition = float4(-Particle.vWorldCurrentScale.x * 0.5f, -Particle.vWorldCurrentScale.y * 0.5, 0.f, 0.f);
    
    
    if (Module.VelocityAlignment)
    {
        output_cross[0].vPosition = float4(-Particle.vWorldCurrentScale.x * 0.5f, 0.f, Particle.vWorldCurrentScale.y * 0.5, 0.f);
        output_cross[1].vPosition = float4(Particle.vWorldCurrentScale.x * 0.5f, 0.f, Particle.vWorldCurrentScale.y * 0.5, 0.f);
        output_cross[2].vPosition = float4(Particle.vWorldCurrentScale.x * 0.5f, 0.f, -Particle.vWorldCurrentScale.y * 0.5, 0.f);
        output_cross[3].vPosition = float4(-Particle.vWorldCurrentScale.x * 0.5f, 0.f, -Particle.vWorldCurrentScale.y * 0.5, 0.f);
            
        
        float3 vR = normalize(mul(float4(Particle.vVelocity, 0.f), g_matView).xyz);
        float3 vF = normalize(cross(vR, float3(0.f, 1.f, 0.f)));
        float3 vU = normalize(cross(vF, vR));

        float3x3 vRot =
        {
            vR,
            vU,
            vF,
        };
        
        for (int i = 0; i < 4; ++i)
        {
            output[i].vPosition.xyz = mul(output[i].vPosition.xyz, vRot);
            output_cross[i].vPosition.xyz = mul(output_cross[i].vPosition.xyz, vRot);
        }
    }
    
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition += vViewPos;        
        output[i].vPosition = mul(output[i].vPosition, g_matProj);
    }
    
    // 생성시킨 정점의 UV 세팅
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    // 생성시킨 정점의 Instance ID 세팅
    output[0].InstID = output[1].InstID = output[2].InstID = output[3].InstID = _in[0].InstID;
   
    
    // OutStream 에 RectMesh 를 만들기 위해서 총 6개의 출력을 지정한다.
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    
    if(Module.VelocityAlignment)
    {
        for (int i = 0; i < 4; ++i)
        {
            output_cross[i].vPosition += vViewPos;
            output_cross[i].vPosition = mul(output_cross[i].vPosition, g_matProj);
        }
    
        // 생성시킨 정점의 UV 세팅
        output_cross[0].vUV = float2(0.f, 0.f);
        output_cross[1].vUV = float2(1.f, 0.f);
        output_cross[2].vUV = float2(1.f, 1.f);
        output_cross[3].vUV = float2(0.f, 1.f);
    
        // 생성시킨 정점의 Instance ID 세팅
        output_cross[0].InstID = output_cross[1].InstID = output_cross[2].InstID = output_cross[3].InstID = _in[0].InstID;
       
        // OutStream 에 RectMesh 를 만들기 위해서 총 6개의 출력을 지정한다.
        _OutStream.Append(output_cross[0]);
        _OutStream.Append(output_cross[1]);
        _OutStream.Append(output_cross[2]);
        _OutStream.RestartStrip();
    
        _OutStream.Append(output_cross[0]);
        _OutStream.Append(output_cross[2]);
        _OutStream.Append(output_cross[3]);
        _OutStream.RestartStrip();
    }    
    
    return;
}


float4 PS_Particle(GS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    
    
    if (g_btex_1)
    {
        vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV);
        vOutColor *= ParticleBuffer[_in.InstID].vColor;        
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    return vOutColor;
}






#endif