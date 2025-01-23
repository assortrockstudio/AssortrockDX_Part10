#ifndef _FUNC
#define _FUNC

#include "value.fx"


float3 CalLight2D(int _LightIdx, float3 _vWorldPos)
{    
    tLightInfo info = g_Light2D[_LightIdx];
  
    // 광원 처리
    float3 vLightPow = (float3) 0.f;
        
    // Directional Light
    if (0 == info.LightType)
    {
        vLightPow = info.Light.vDiffuse.rgb + info.Light.vAmbient.rgb;
    }
    
    // PointLight
    else if (1 == info.LightType)
    {
        // 픽셀의 월드스페이스
        float fDist = distance(info.WorldPos.xy, _vWorldPos.xy);
        
        float fRatio = 0.f;
       
        //fRatio = 1.f - saturate(fDist / info.Range);
        fRatio = cos((fDist / info.Range) * (PI / 2.f));        
        
        if (fDist < info.Range)
        {
            vLightPow = info.Light.vDiffuse.xyz * fRatio;
        }
    }
    
    // SpotLight
    else
    {
        
    }
        
    return vLightPow;
}

void CalLight3D(int _LightIdx, float3 _ViewPos, float3 _ViewNormal, inout tLight _Light)
{    
    // 광원의 정보
    tLightInfo LightInfo = g_Light3D[_LightIdx];
        
    // 빛의 세기
    float LightPow = (float) 0.f;
    
    // 빛의 방향
    float3 vLight = (float3) 0.f;
    
    // 빛을 적용받을 픽셀(물체) 에서의 법선벡터
    _ViewNormal = normalize(_ViewNormal);
    
    // 거리에 따른 빛의 세기
    float DistancePow = 1.f;
    
    // Directional
    if (0 == LightInfo.LightType)
    {
        // View 스페이스에서 광원의 방향
        vLight = normalize(mul(float4(LightInfo.WorldDir, 0.f), g_matView).xyz);        
        
        // 램버트 코사인법칙
        LightPow = saturate(dot(_ViewNormal, -vLight));
    }
    
    // Point
    else if(1 == LightInfo.LightType)
    {
        // View 스페이스에서 광원의 방향
        // 물체(픽셀) 의 위치 - 광원의 위치
        float3 vLightPos = mul(float4(LightInfo.WorldPos.xyz, 1.f), g_matView).xyz;       
        vLight = normalize(_ViewPos - vLightPos);
        
        // 램버트 코사인법칙
        LightPow = saturate(dot(_ViewNormal, -vLight));
        
        // 광원과 물체 사이의 거리를 구한다.
        float Distance = distance(_ViewPos, vLightPos);
        
        // 거리에 따른 빛의 세기 감소율
        //DistancePow =  saturate(1.f - Distance / LightInfo.Range);
        DistancePow = saturate(cos(saturate(Distance / LightInfo.Range) * (PI / 2.f)));
    }
    
    // Spot
    else
    {
        
    } 
    
    // 반사
    // Reflect
    float3 vReflect = normalize(vLight + dot(-vLight, _ViewNormal) * 2.f * _ViewNormal);
        
    // 시선벡터
    float3 vEye = normalize(_ViewPos);
    
    // 반사된 빛이 카메라로 들어오는 각도를 cos0 로 환산
    float ReflectPow = saturate(dot(-vEye, vReflect));
    ReflectPow = pow(ReflectPow, 20);
         
    // 빛의 크기를 계산
    _Light.vDiffuse.rgb     += LightInfo.Light.vDiffuse.rgb * LightPow * DistancePow;
    _Light.vAmbient.rgb     += LightInfo.Light.vAmbient.rgb;
    _Light.vMaxSpecular.rgb += LightInfo.Light.vDiffuse.rgb * LightInfo.Light.vMaxSpecular.rgb * ReflectPow * DistancePow;
}


int IsBinding(in Texture2D _tex)
{
    uint width = 0;
    uint height = 0;
    _tex.GetDimensions(width, height);
    
    if (width && height)
        return 1;
    else
        return 0;
}



float3 GetRandom(in Texture2D _Noise, float _NormalizedThreadID)
{
    // 파티클의 포지션을 랜덤한 위치로 지정하기
    float2 vUV = (float2) 0.f;      
    
    vUV.x = _NormalizedThreadID + (Time * 0.1f);
    vUV.y = (sin((vUV.x - Time) * PI * 20.f) * 0.5f) + (Time * 0.2f);
                
    float3 vNoise = _Noise.SampleLevel(g_sam_0, vUV, 0).xyz;
    
    return vNoise;
}

float GetTessFactor(float _MinLevel, float _MaxLevel
                  , float _MinRange, float _MaxRange
                  , float3 _CamPos, float3 _Pos)
{
    float D = distance(_CamPos, _Pos);
    
    if (D < _MaxRange)
        return pow(2.f, _MaxLevel);
    else if (_MinRange < D)
        return pow(2.f, _MinLevel);
    else
    {
        float fRatio = 1.f - (D - _MaxRange) / abs(_MaxRange - _MinRange);
        
        float Level = 1.f + fRatio * (_MaxLevel - _MinLevel - 1.f);
        
        return pow(2.f, Level);
    }
     
}


int IntersectsRay(float3 _Pos[3], float3 _vStart, float3 _vDir
                  , out float3 _CrossPos, out uint _Dist)
{
    // 삼각형 표면 방향 벡터
    float3 Edge[2] = { (float3) 0.f, (float3) 0.f };
    Edge[0] = _Pos[1] - _Pos[0];
    Edge[1] = _Pos[2] - _Pos[0];
    
    // 삼각형에 수직방향인 법선(Normal) 벡터
    float3 Normal = normalize(cross(Edge[0], Edge[1]));
    
    // 삼각형 법선벡터와 Ray 의 Dir 을 내적
    float NdotD = -dot(Normal, _vDir); // 광선에서 삼각형으로 향하는 수직벡터와, 광선의 방향벡터 사이의 cos 값
        
    float3 vStoP0 = _vStart - _Pos[0];
    float VerticalDist = dot(Normal, vStoP0); // 광선을 지나는 한점에서 삼각형 평면으로의 수직 길이
            
    // 광선이 진행하는 방향으로, 삼각형을 포함하는 평면까지의 거리
    float RtoTriDist = VerticalDist / NdotD;
        
    // 광선이 삼각형을 포함하는 평면을 지나는 교점
    float3 vCrossPoint = _vStart + RtoTriDist * _vDir;
        
    // 교점이 삼각형 내부인지 테스트
    float3 P0toCross = vCrossPoint - _Pos[0];
    
    float Full = length(cross(Edge[0], Edge[1]));
    float U = length(cross(P0toCross, Edge[0]));
    float V = length(cross(P0toCross, Edge[1]));
        
    if (Full < U + V)
        return 0;
        
    _CrossPos = vCrossPoint;
    _Dist = (uint) RtoTriDist;
    
    return 1;
}


matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices
    , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}

#endif