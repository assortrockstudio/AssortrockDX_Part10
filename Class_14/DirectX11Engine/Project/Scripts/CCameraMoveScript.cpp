#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine/CCamera.h>

CCameraMoveScript::CCameraMoveScript()
	: CScript(CAMERAMOVESCRIPT)
	, m_Speed(200.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
	// 카메라 투영 모드 변경
	if (KEY_TAP(KEY::P))
	{
		Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC ? Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE) : Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
		{
			Vec3 vPos = Transform()->GetRelativePos();
			Transform()->SetRelativePos(Vec3(vPos.x, vPos.y, 0.f));
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
	}

	if (Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE)
		MoveByPerspective();
	else
		MoveByOrthographic();	
}


void CCameraMoveScript::MoveByPerspective()
{
	// Shift 속도 배율
	float Speed = m_Speed;
	if (KEY_PRESSED(KEY::LSHILFT))
	{
		Speed *= 4.f;
	}

	// 키 입력에 따른 위치이동
	Vec3 vCurPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	if (KEY_PRESSED(KEY::W))
	{
		vCurPos += vFront * Speed * DT;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vCurPos += -vFront * Speed * DT;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vCurPos += -vRight * DT * Speed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vCurPos += vRight * DT * Speed;
	}

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();

		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += vDrag.x * DT * XM_PI * 20.f;
		vRot.x -= vDrag.y * DT * XM_PI * 20.f;

		Transform()->SetRelativeRotation(vRot);
	}

	GetOwner()->Transform()->SetRelativePos(vCurPos);
}

void CCameraMoveScript::MoveByOrthographic()
{
	// Shift 속도 배율
	float Speed = m_Speed;
	if (KEY_PRESSED(KEY::LSHILFT))
	{
		Speed *= 4.f;
	}

	// 키 입력에 따른 위치이동
	Vec3 vCurPos = Transform()->GetRelativePos();
	
	if (KEY_PRESSED(KEY::W))
	{
		vCurPos.y += Speed * DT;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vCurPos.y -= Speed * DT;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vCurPos.x -= Speed * DT;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vCurPos.x += Speed * DT;
	}

	if (KEY_PRESSED(KEY::NUM1))
	{
		float scale = Camera()->GetScale();
		scale -= DT;

		if (scale < 0.01f)
		{
			scale = 0.01f;
		}

		Camera()->SetScale(scale);
	}

	if (KEY_PRESSED(KEY::NUM2))
	{
		float scale = Camera()->GetScale();
		scale += DT;
		Camera()->SetScale(scale);
	}

	GetOwner()->Transform()->SetRelativePos(vCurPos);
}