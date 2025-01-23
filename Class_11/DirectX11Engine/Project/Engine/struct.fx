#ifndef _STRUCT
#define _STRUCT

struct tLight
{
    float4 vDiffuse; // 난반사 (빛의 색상)
    float4 vAmbient; // 환경광 (빛으로 발생하는 최소한의 밝기)
    float4 vMaxSpecular; // 반사광 (빛으로 발생하는 반사광의 최대 밝기)
};

struct tLightInfo
{
    tLight  Light;  // 광원 색상 정보

    uint    LightType; // 0 : DirectionalLight, 1 : PointLight, 2 : SpotLight
    float3  WorldDir; // WorldSpace 에서 광원의 방향
    float3  WorldPos; // WorldSpace 에서 광원의 위치   
    float   Range; // SpotLight or PointLight 에 광원의 영향 범위
    float   Angle; // Spot Light 인 경우 광원의 각도범위
    float3  Padding;
};


struct tParticle
{
    float4  vColor;
    float3  vLocalPos;
    float3  vWorldPos;
    float3  vWorldInitScale;
    float3  vWorldCurrentScale;
    float3  vWorldRotation;
    
    float3  vForce;
    float3  vVelocity;
        
    float   NoiseForceAccTime;  // Noise Force 텀에 도달하는 누적 시간
    float3  NoiseForceDir;      // 적용된 Noise Forec 의 방향
            
    float   Mass;
    float   Life;
    float   Age;
    float   NomarlizedAge;
    int     Active;
    
    float2  padding;
};

struct tSpawnCount
{
    int spawncount;
    uint3 padding;
};

struct tParticleModule
{
    // Spawn
    uint    SpawnRate; // 초당 파티클 생성 개수
    float4  vSpawnColor; // 생성 시점 색상
    float4  vSpawnMinScale; // 생성 시 최소 크기
    float4  vSpawnMaxScale; // 생성 시 최대 크기

    float   MinLife;            // 최소 수명
    float   MaxLife;            // 최대 수명

    uint    SpawnShape;         // 0 : Box,  1: Sphere
    float3  SpawnShapeScale;    // SpawnShapeScale.x == Radius

    uint    BlockSpawnShape;    // 0 : Box,  1: Sphere
    float3  BlockSpawnShapeScale; // SpawnShapeScale.x == Radius    
    
    uint    SpaceType;          // 0 : LocalSpcae, 1 : WorldSpace
    
    // Spawn Burst
    uint    SpawnBurstCount;    // 한번에 발생시키는 Particle 수
    uint    SpawnBurstRepeat;
    float   SpawnBurstRepeatTime;
    
    // Add Velocity
    uint    AddVelocityType;    // 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
    float3  AddVelocityFixedDir;
    float   AddMinSpeed;
    float   AddMaxSpeed;
    
    // Scale Module
    float   StartScale;
    float   EndScale;
    
    // Drag Module
    float   DestNormalizedAge;
    float   LimitSpeed;    
    
    // Noise Force Module
    float   NoiseForceTerm;     // Noise Force 적용시키는 텀
    float   NoiseForceScale;    // Noise Force 의 크기
       
    // Render Module
    float3  EndColor;           // 최종 색상
    uint    FadeOut;            // 0 : Off, 1 : Normalized Age
    float   StartRatio;         // FadeOut 효과가 시작되는 Normalized Age 지점
    uint    VelocityAlignment;  // 속도 정렬 0 : Off, 1 : On
    
	// Module On / Off
    int     Module[7];
};


struct tRaycastOut
{
    float2  Location;
    uint    Distance;
    int     Success;
};

struct tWeight
{
    float arrWeight[8];
};



#endif