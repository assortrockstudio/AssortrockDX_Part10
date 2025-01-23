#ifndef _COMPUTE
#define _COMPUTE

#include "value.fx"

// hlsl 5.0 기준 최대 그룹당 지정 가능한 스레드 개수는 1024개이다.

// SV_GroupThreadID     : 그룹 내에서의 스레드의 3차원 좌표
// SV_GroupIndex        : 그룹 내에서 스레드의 1차원 좌표
// SV_GroupID           : 스레드가 속한 그룹의 3차원 좌표
// SV_DispatchThreadID  : 모든 그룹을 포함한 전체 기준 3차원 좌표

RWTexture2D<float4> TargetTex : register(u0); // unordered register

#define Width   g_int_0
#define Height  g_int_1
#define Color   g_vec4_0

[numthreads(32, 32, 1)]
void CS_Test(int3 _ThreadID : SV_DispatchThreadID)
{        
    if (Width <= _ThreadID.x || Height <= _ThreadID.y)
    {
        return;
    }    
    
    TargetTex[_ThreadID.xy] = Color;
}

#endif