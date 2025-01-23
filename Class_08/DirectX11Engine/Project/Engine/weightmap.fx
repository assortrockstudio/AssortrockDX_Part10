#ifndef _WEIGHTMAP
#define _WEIGHTMAP

#include "value.fx"
#include "struct.fx"



RWStructuredBuffer<tWeight>     WEIGHT_MAP : register(u0);
StructuredBuffer<tRaycastOut>   Raycast : register(t20);

#define WIDTH                   g_int_0
#define HEIGHT                  g_int_1
#define WEIGHT_IDX              g_int_2

#define BRUSH_TEX               g_tex_0
#define BRUSH_SCALE             g_vec2_0

[numthreads(32, 32, 1)]
void CS_WeightMap(int3 _ID : SV_DispatchThreadID)
{
    if (WIDTH <= _ID.x || HEIGHT <= _ID.y || !Raycast[0].Success)
        return;
    
    
}
#endif