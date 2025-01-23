#include "pch.h"
#include "CParticleSystem.h"

#include "CTransform.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CTimeMgr.h"


CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_Time(0.f)
	, m_MaxParticle(1000)
{
	SetFrustumCheck(false);

	// Particle Tick 용도 ComputeShader 
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleCS").Get();

	// ParticleSystem 이 사용할 메시와 머티리얼 설정
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl"));

	// 파티클 정보를 저장할 구조화 버퍼 생성	
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false, nullptr);

	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true);

	// 파티클 기능(모듈) 정보 세팅
	// Spawn Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	m_Module.SpawnRate = 20;
	m_Module.vSpawnColor = Vec4(0.24f, 0.67f, 0.87f, 1.f);
	m_Module.MinLife = 5.f;
	m_Module.MaxLife = 5.f;
	m_Module.vSpawnMinScale = Vec3(80.f, 10.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(80.f, 10.f, 1.f);

	m_Module.SpawnShape = 1;
	m_Module.SpawnShapeScale.x = 300.f;

	m_Module.BlockSpawnShape = 1;
	m_Module.BlockSpawnShapeScale.x = 0.f;

	m_Module.SpaceType = 1; // Local Space 

	// Spawn Burst Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;
	m_Module.SpawnBurstRepeat = true;
	m_Module.SpawnBurstCount = 100;
	m_Module.SpawnBurstRepeatTime = 3.f;

	// AddVelocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_Module.AddVelocityType = 1;
	m_Module.AddVelocityFixedDir = Vec3(0.f, 1.f, 0.f);
	m_Module.AddMinSpeed = 300.f;
	m_Module.AddMaxSpeed = 800.f;

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	m_Module.StartScale = 2.f;
	m_Module.EndScale = 2.f;

	// Drag Module
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = true;
	m_Module.DestNormalizedAge = 0.5f;
	m_Module.LimitSpeed = 0.f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_Module.NoiseForceTerm = 0.2f;
	m_Module.NoiseForceScale = 50.f;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_Module.EndColor = Vec3(1.f, 0.f, 0.f);
	m_Module.FadeOut = true;
	m_Module.FadeOutStartRatio = 0.9f;
	m_Module.VelocityAlignment = true;

	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule) + (16 % (16 - sizeof(tParticleModule) % 16) ), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _Other)
	: CRenderComponent(_Other)
	, m_TickCS(_Other.m_TickCS)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_ModuleBuffer(nullptr)
	, m_ParticleTex(_Other.m_ParticleTex)
	, m_Time(0.f)
	, m_BurstTime(0.f)
	, m_MaxParticle(_Other.m_MaxParticle)
	, m_Module(_Other.m_Module)
{
	assert(_Other.m_ParticleBuffer && _Other.m_SpawnCountBuffer && _Other.m_ModuleBuffer);

	m_ParticleBuffer = new CStructuredBuffer(*_Other.m_ParticleBuffer);
	m_SpawnCountBuffer = new CStructuredBuffer(*_Other.m_SpawnCountBuffer);
	m_ModuleBuffer = new CStructuredBuffer(*_Other.m_ModuleBuffer);
}

CParticleSystem::~CParticleSystem()
{
	delete m_ParticleBuffer;
	delete m_SpawnCountBuffer;
	delete m_ModuleBuffer;
}


void CParticleSystem::finaltick()
{	
	// SpawnCount 계산
	CaculateSpawnCount();

	// ComputeShader Excecute
	m_TickCS->SetParticleWorldPos(Transform()->GetWorldPos());
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCount(m_SpawnCountBuffer);
	

	if (FAILED(m_TickCS->Execute()))
	{
		assert(nullptr);
	}	
}

void CParticleSystem::render()
{
	// Resource Binding
	m_ParticleBuffer->Binding(17);
	m_ModuleBuffer->Binding(18);
	Transform()->Binding();

	GetMaterial()->SetTexParam(TEX_1, m_ParticleTex);
	GetMaterial()->Binding();


	// 모든 입자를 한번에 인스턴싱으로 랜더링함
	GetMesh()->render_particle(m_MaxParticle);


	// Resource Clear
	m_ParticleBuffer->Clear_SRV();
	m_ModuleBuffer->Clear_SRV();
}



void CParticleSystem::SetMaxParticleCount(UINT _Max)
{
	m_MaxParticle = _Max;

	if (m_ParticleBuffer->GetElementCount() < m_MaxParticle)
	{
		m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false, nullptr);
	}
}

void CParticleSystem::CaculateSpawnCount()
{
	m_Time += DT;
	tSpawnCount count = {  };

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN])
	{
		// SpawnRate 에 따른 이번 Tick 에서 생성시킬 파티클의 수
		float Term = 1.f / (float)m_Module.SpawnRate;
		UINT SpawnCount = 0;

		if (Term < m_Time)
		{
			float Value = m_Time / Term;
			SpawnCount = (UINT)Value;
			m_Time -= (float)SpawnCount * Term;
		}

		count.SpawnCount = SpawnCount;
	}
	
	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST])
	{	
		UINT BurstCount = 0;

		if (0.f == m_BurstTime)
		{
			BurstCount = m_Module.SpawnBurstCount;
		}		
		
		m_BurstTime += DT;

		if (m_Module.SpawnBurstRepeat && m_Module.SpawnBurstRepeatTime <= m_BurstTime)
		{
			m_BurstTime -= m_Module.SpawnBurstRepeatTime;
			BurstCount += m_Module.SpawnBurstCount;
		}

		count.SpawnCount += BurstCount;
	}	

	// SpawnCount 를 Buffer 에 전달	
	m_SpawnCountBuffer->SetData(&count);
}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
		
	SaveAssetRef<CComputeShader>((CComputeShader*)m_TickCS.Get(), _File);	
	SaveAssetRef(m_ParticleTex, _File);
		
	fwrite(&m_MaxParticle, sizeof(UINT), 1, _File);	
	fwrite(&m_Module, sizeof(tParticleModule), 1, _File);	
}

void CParticleSystem::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	Ptr<CComputeShader> pTickCS = m_TickCS.Get();
	LoadAssetRef(pTickCS, _File);
	m_TickCS = (CParticleTickCS*)pTickCS.Get();

	LoadAssetRef(m_ParticleTex, _File);

	fread(&m_MaxParticle, sizeof(UINT), 1, _File);
	SetMaxParticleCount(m_MaxParticle);

	fread(&m_Module, sizeof(tParticleModule), 1, _File);
}