#ifndef _FUNC
#define _FUNC

#include "value.fx"


float3 CalLight2D(int _LightIdx, float3 _vWorldPos)
{    
    tLightInfo info = g_Light2D[_LightIdx];
  
    // ���� ó��
    float3 vLightPow = (float3) 0.f;
        
    // Directional Light
    if (0 == info.LightType)
    {
        vLightPow = info.Light.vDiffuse.rgb + info.Light.vAmbient.rgb;
    }
    
    // PointLight
    else if (1 == info.LightType)
    {
        // �ȼ��� ���彺���̽�
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
    // ������ ����
    tLightInfo LightInfo = g_Light3D[_LightIdx];
        
    // ���� ����
    float LightPow = (float) 0.f;
    
    // ���� ����
    float3 vLight = (float3) 0.f;
    
    // ���� ������� �ȼ�(��ü) ������ ��������
    _ViewNormal = normalize(_ViewNormal);
    
    // �Ÿ��� ���� ���� ����
    float DistancePow = 1.f;
    
    // Directional
    if (0 == LightInfo.LightType)
    {
        // View �����̽����� ������ ����
        vLight = normalize(mul(float4(LightInfo.WorldDir, 0.f), g_matView).xyz);        
        
        // ����Ʈ �ڻ��ι�Ģ
        LightPow = saturate(dot(_ViewNormal, -vLight));
    }
    
    // Point
    else if(1 == LightInfo.LightType)
    {
        // View �����̽����� ������ ����
        // ��ü(�ȼ�) �� ��ġ - ������ ��ġ
        float3 vLightPos = mul(float4(LightInfo.WorldPos.xyz, 1.f), g_matView).xyz;       
        vLight = normalize(_ViewPos - vLightPos);
        
        // ����Ʈ �ڻ��ι�Ģ
        LightPow = saturate(dot(_ViewNormal, -vLight));
        
        // ������ ��ü ������ �Ÿ��� ���Ѵ�.
        float Distance = distance(_ViewPos, vLightPos);
        
        // �Ÿ��� ���� ���� ���� ������
        //DistancePow =  saturate(1.f - Distance / LightInfo.Range);
        DistancePow = saturate(cos(saturate(Distance / LightInfo.Range) * (PI / 2.f)));
    }
    
    // Spot
    else
    {
        
    } 
    
    // �ݻ�
    // Reflect
    float3 vReflect = normalize(vLight + dot(-vLight, _ViewNormal) * 2.f * _ViewNormal);
        
    // �ü�����
    float3 vEye = normalize(_ViewPos);
    
    // �ݻ�� ���� ī�޶�� ������ ������ cos0 �� ȯ��
    float ReflectPow = saturate(dot(-vEye, vReflect));
    ReflectPow = pow(ReflectPow, 20);
         
    // ���� ũ�⸦ ���
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
    // ��ƼŬ�� �������� ������ ��ġ�� �����ϱ�
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
    // �ﰢ�� ǥ�� ���� ����
    float3 Edge[2] = { (float3) 0.f, (float3) 0.f };
    Edge[0] = _Pos[1] - _Pos[0];
    Edge[1] = _Pos[2] - _Pos[0];
    
    // �ﰢ���� ���������� ����(Normal) ����
    float3 Normal = normalize(cross(Edge[0], Edge[1]));
    
    // �ﰢ�� �������Ϳ� Ray �� Dir �� ����
    float NdotD = -dot(Normal, _vDir); // �������� �ﰢ������ ���ϴ� �������Ϳ�, ������ ���⺤�� ������ cos ��
        
    float3 vStoP0 = _vStart - _Pos[0];
    float VerticalDist = dot(Normal, vStoP0); // ������ ������ �������� �ﰢ�� ��������� ���� ����
            
    // ������ �����ϴ� ��������, �ﰢ���� �����ϴ� �������� �Ÿ�
    float RtoTriDist = VerticalDist / NdotD;
        
    // ������ �ﰢ���� �����ϴ� ����� ������ ����
    float3 vCrossPoint = _vStart + RtoTriDist * _vDir;
        
    // ������ �ﰢ�� �������� �׽�Ʈ
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