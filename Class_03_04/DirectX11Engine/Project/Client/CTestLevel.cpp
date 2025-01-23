#include "pch.h"

#include "CTestLevel.h"

#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CAnim2D.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CCameraMoveScript.h>
#include <Scripts/CBackgroundScript.h>
#include <Scripts/CMissileScript.h>
		 
#include <Engine/CCollisionMgr.h>
#include <Engine/CSetColorCS.h>
		 
#include <Engine/CStructuredBuffer.h>
#include <Engine/CPrefab.h>


void CTestLevel::CreateTestLevel()
{
	CLevel* pLevel = nullptr;
	
	// 임시 레벨 설정
	pLevel = new CLevel;
	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Player");
	pLevel->GetLayer(2)->SetName(L"Monster");

	ChangeLevel(pLevel, LEVEL_STATE::STOP);
		
	// Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->SetCameraPriority(0); // 메인 카메라로 설정
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pLevel->AddObject(0, pCamObj);
	
	// ============
	// Light3D 광원
	// ============
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Directional Light");

	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight3D);

	pLight->Transform()->SetRelativePos(400.f, 100.f, 500.f);
	pLight->Transform()->SetRelativeRotation(XM_PI / 4.f, XM_PI / 4.f, 0.f);

	pLight->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));	
	pLight->Light3D()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));
	pLight->Light3D()->SetRange(600.f);
	pLight->Light3D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));

	pLevel->AddObject(0, pLight);
		

	// ======
	// SkyBox 
	// ======
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativeScale(1000.f, 1000.f, 1000.f);

	pSkyBox->SkyBox()->SetSkyBoxType(SPHERE);
	pSkyBox->SkyBox()->SetSkyBoxTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\skybox\\Sky02.jpg"));

	//pSkyBox->SkyBox()->SetSkyBoxType(CUBE);
	//pSkyBox->SkyBox()->SetSkyBoxTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\skybox\\SkyWater.dds"));

	pLevel->AddObject(0, pSkyBox);


	// ======
	// Player
	// ======
	CGameObject* Player = new CGameObject;
	Player->SetName(L"Player");

	Player->AddComponent(new CTransform);
	Player->AddComponent(new CMeshRender);
	Player->AddComponent(new CBoundingBox);
	
	Player->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	Player->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1000.f));
	//Player->Transform()->SetRelativeRotation(Vec3(XM_PI / 2.f, 0.f, 0.f));

	Player->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Player->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TessMtrl"));
	Player->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\Background.jpg"));

	Player->BoundingBox()->SetAbsolute(true);
	Player->BoundingBox()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	Player->BoundingBox()->SetRaidus(300.f);

	pLevel->AddObject(0, Player);
			
	// Decal
	CGameObject* pDecalObj = new CGameObject;
	pDecalObj->SetName(L"Decal");

	pDecalObj->AddComponent(new CTransform);
	pDecalObj->AddComponent(new CDecal);

	pDecalObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 200.f));
	pDecalObj->Decal()->SetOutputTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\Character.png"));
	pDecalObj->Decal()->SetAsLight(true);
	pDecalObj->Decal()->SetDecalPow(0.8f);

	pLevel->AddObject(0, pDecalObj);
	

	// LandScape
	CGameObject* pLandScape = new CGameObject;
	pLandScape->SetName(L"LandScape");

	pLandScape->AddComponent(new CTransform);
	pLandScape->AddComponent(new CLandScape);

	pLandScape->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));

	pLandScape->LandScape()->SetFace(32, 32);
	pLandScape->LandScape()->CreateHeightMap(1024, 1024);
	//pLandScape->LandScape()->SetHeightMap(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\HeightMap_01.jpg"));


	pLevel->AddObject(0, pLandScape);	
}





void CTestLevel::CreatePrefab()
{
	// Particle Prefab
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"Particle");
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pParticleObj->ParticleSystem()->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\particle\\AlphaCircle.png", L"texture\\particle\\AlphaCircle.png"));

	Ptr<CPrefab> Prefab = new CPrefab(pParticleObj);
	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"ParticlePrefab", Prefab);

	wstring FilePath = CPathMgr::GetInst()->GetContentPath();
	Prefab->Save(FilePath + L"prefab\\Particle.pref");



	// Missile Prefab 
	CGameObject* pNewObj = new CGameObject;
	pNewObj->SetName(L"Missile");
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRender);
	pNewObj->AddComponent(new CMissileScript);
	pNewObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 40.f));
	pNewObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pNewObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	Prefab = new CPrefab(pNewObj);
	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePrefab", Prefab);
	Prefab->Save(FilePath + L"prefab\\Missile.pref");
}