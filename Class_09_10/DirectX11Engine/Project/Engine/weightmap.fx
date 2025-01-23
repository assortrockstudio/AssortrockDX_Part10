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
    
    // 마우스 피킹위치를 2차원 인덱스로 변환
    int2 CenterPos = float2(WIDTH, HEIGHT) * Raycast[0].Location;
    
    // 브러시 범위를 해상도 단위로 변경
    int2 Scale = float2(WIDTH, HEIGHT) * BRUSH_SCALE;    
       
    // 마우스 피킹 위치를 중심으로 브러쉬 범위 영역을 벗어난 부위를 담당하는 스레드들은 종료한다.
    if (_ID.x < CenterPos.x - (Scale.x / 2) || CenterPos.x + (Scale.x / 2) < _ID.x
        || _ID.y < CenterPos.y - (Scale.y / 2) || CenterPos.y + (Scale.y / 2) < _ID.y)
    {
        return;
    }
        
    // (Thread 좌표 - 좌상단 좌표) / 브러쉬 스케일 ==> 해당 스레드가 참조해야하는 브러시텍스쳐 UV 위치
    int2 LeftTop = CenterPos - (Scale / 2);
    float2 vBrushUV = (float2)(_ID.xy - LeftTop) / (float2)Scale;
    float Alpha = BRUSH_TEX.SampleLevel(g_sam_0, vBrushUV, 0).a;
        
    // Thread 가 담당한 WEIGHT_MAP 에서의 1차원 인덱스를 구한다.
    int WeightMapIdx = WIDTH * _ID.y + _ID.x;
    WEIGHT_MAP[WeightMapIdx].arrWeight[WEIGHT_IDX] = saturate(WEIGHT_MAP[WeightMapIdx].arrWeight[WEIGHT_IDX] + 1.f * DT_Engine * Alpha);
}
#endif