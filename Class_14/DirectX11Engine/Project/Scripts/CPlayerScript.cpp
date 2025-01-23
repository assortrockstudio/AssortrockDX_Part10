#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <Engine/CCollider2D.h>

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_Speed(500.f)	
	, m_TargetMon(nullptr)
{	
	AddScriptProperty(PROPERTY_TYPE::FLOAT, "Player Speed", &m_Speed);
}

CPlayerScript::~CPlayerScript()
{	
}

void CPlayerScript::begin()
{
	//m_ParticlePrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"ParticlePrefab");
	m_MissilePrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Missile.pref");
}

void CPlayerScript::tick()
{
	// 키 입력에 따른 위치이동
	Vec3 vCurPos = Transform()->GetRelativePos();

	if (KEY_TAP(KEY::U))
	{
		Ptr<CMaterial> pMtrl = GetRenderComponent()->GetDynamicMaterial();
		pMtrl->SetScalarParam(INT_0, 1);
	}
	else if(KEY_RELEASED(KEY::U))
	{
		GetRenderComponent()->RestoreMaterial();
	}


	if (KEY_PRESSED(KEY::UP))
	{
		vCurPos.y += DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		vCurPos.y -= DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		vCurPos.x -= DT * m_Speed;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vCurPos.x += DT * m_Speed;
	}

	if (KEY_TAP(KEY::ALT))
	{
		Instantiate(m_MissilePrefab, 0, Transform()->GetRelativePos());
	}

	Transform()->SetRelativePos(vCurPos);
}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{	
	
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	int a = 0;
}

void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
	SaveAssetRef(m_ParticlePrefab, _File);
	SaveAssetRef(m_MissilePrefab, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
	LoadAssetRef(m_ParticlePrefab, _File);
	LoadAssetRef(m_MissilePrefab, _File);
}
