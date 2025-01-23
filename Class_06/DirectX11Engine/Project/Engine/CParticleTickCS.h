#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CParticleTickCS :
    public CComputeShader
{
private:
    CStructuredBuffer*  m_ParticleBuffer;
    CStructuredBuffer*  m_SpawnCountBuffer;
    CStructuredBuffer*  m_ModuleBuffer;
    Ptr<CTexture>       m_NoiseTex;

    Vec3                m_vParticleWorldPos;


public:
    void SetModuleBuffer(CStructuredBuffer* _ModuleBuffer) { m_ModuleBuffer = _ModuleBuffer; }
    void SetParticleBuffer(CStructuredBuffer* _Buffer) { m_ParticleBuffer = _Buffer; }
    void SetSpawnCount(CStructuredBuffer* _Buffer) { m_SpawnCountBuffer = _Buffer; }
    void SetParticleWorldPos(Vec3 _vPos) { m_vParticleWorldPos = _vPos; } 

public:
    virtual int Binding() override;
    virtual void CalculateGroupNum()override;
    virtual void Clear() override;

public:
    CParticleTickCS();
    ~CParticleTickCS();
};

