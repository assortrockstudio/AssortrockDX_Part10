#include "pch.h"
#include "CScript.h"

#include "CTransform.h"

CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptType(_ScriptType)
{
}

CScript::~CScript()
{
}

CGameObject* CScript::Instantiate(Ptr<CPrefab> _Prefab, int _LayerIdx, const Vec3& _WorldPos)
{
	CGameObject* pInst = _Prefab->Instantiate();
	pInst->Transform()->SetRelativePos(_WorldPos);
	SpawnObject(_LayerIdx, pInst);

	return pInst;
}
