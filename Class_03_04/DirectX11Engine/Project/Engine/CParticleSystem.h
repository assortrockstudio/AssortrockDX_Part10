#pragma once
#include "CRenderComponent.h"

#include "CParticleTickCS.h"


class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleTickCS>    m_TickCS;
    CStructuredBuffer*      m_ParticleBuffer;   // ParticleBuffer
    CStructuredBuffer*      m_SpawnCountBuffer; // SpawnCount 전달 용도
    CStructuredBuffer*      m_ModuleBuffer;     // Module Data Buffer

    Ptr<CTexture>           m_ParticleTex;      // 파티클 입자 텍스쳐
    float                   m_Time;             // 누적 시간
    float                   m_BurstTime;        // SpawnBurst 체크용 변수

    // Particle Data
    UINT                    m_MaxParticle;      // 파티클 최대 개수    

    tParticleModule         m_Module;           // 파티클의 기능 정의

public:
    void SetParticleTexture(Ptr<CTexture> _Tex) { m_ParticleTex = _Tex; }
    void SetMaxParticleCount(UINT _Max);

private:
    void CaculateSpawnCount();

public:
    virtual void finaltick() override;
    virtual void render() override;

    CLONE(CParticleSystem);
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _Other);
    ~CParticleSystem();
};

