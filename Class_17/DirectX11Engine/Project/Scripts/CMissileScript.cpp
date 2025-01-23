#include "pch.h"
#include "CMissileScript.h"

CMissileScript::CMissileScript()
	: CScript(MISSILESCRIPT)
	, m_Speed(500.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::begin()
{
	GetRenderComponent()->GetDynamicMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Effect.png", L"texture\\Effect.png"));
}

void CMissileScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	vPos.y += m_Speed * DT;

	Transform()->SetRelativePos(vPos);
}

