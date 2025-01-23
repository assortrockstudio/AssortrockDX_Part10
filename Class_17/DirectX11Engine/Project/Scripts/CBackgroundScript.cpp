#include "pch.h"
#include "CBackgroundScript.h"


CBackgroundScript::CBackgroundScript()
	: CScript(SCRIPT_TYPE::BACKGROUNDSCRIPT)
	, m_Intence(0.f)
{
}

CBackgroundScript::~CBackgroundScript()
{
}

void CBackgroundScript::tick()
{
	//m_Intence += DT * 0.2f;
	//GetRenderComponent()->GetMaterial()->SetScalarParam(FLOAT_0, m_Intence);
}