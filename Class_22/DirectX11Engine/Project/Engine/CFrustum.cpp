#include "pch.h"
#include "CFrustum.h"

#include "CCamera.h"


CFrustum::CFrustum(CCamera* _Owner)
	: m_Camera(_Owner)
{
	// Frustum 초기 값 세팅
	//    4 --- 5
	//   /|  \  |  Far
	//  / 7 --/ 6     
	// 0 -- 1/ /
	// | \  | / 
	// 3 -- 2  Near
	m_arrProjPos[0] = Vec3(-1.f, 1.f, 0.f);
	m_arrProjPos[1] = Vec3(1.f, 1.f, 0.f);
	m_arrProjPos[2] = Vec3(1.f, -1.f, 0.f);
	m_arrProjPos[3] = Vec3(-1.f, -1.f, 0.f);

	m_arrProjPos[4] = Vec3(-1.f, 1.f, 1.f);
	m_arrProjPos[5] = Vec3(1.f, 1.f, 1.f);
	m_arrProjPos[6] = Vec3(1.f, -1.f, 1.f);
	m_arrProjPos[7] = Vec3(-1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::finaltick()
{
	const Matrix& matViewInv = m_Camera->GetViewInvMat();
	const Matrix& matProjInv = m_Camera->GetProjInvMat();
	Matrix matInv = matProjInv * matViewInv;
		
	// 투영공간 끝좌표들 -> ViewSpace -> WorldSpcae
	Vec3 arrWorld[8] = {};
	
	for (int i = 0; i < 8; ++i)
	{
		arrWorld[i] = XMVector3TransformCoord(m_arrProjPos[i], matInv);
	}

	// 6개의 카메라 시야범위 평면을 월드공간 기준으로 만들어낸다.
	// Frustum 초기 값 세팅
	//    4 --- 5
	//   /|  \  |  Far
	//  / 7 --/ 6     
	// 0 -- 1/ /
	// | \  | / 
	// 3 -- 2  Near
	m_arrFace[FACE_NEAR] = XMPlaneFromPoints(arrWorld[0], arrWorld[1], arrWorld[2]);
	m_arrFace[FACE_FAR] = XMPlaneFromPoints(arrWorld[5], arrWorld[4], arrWorld[7]);

	m_arrFace[FACE_LEFT] = XMPlaneFromPoints(arrWorld[7], arrWorld[4], arrWorld[0]);
	m_arrFace[FACE_RIGHT] = XMPlaneFromPoints(arrWorld[1], arrWorld[5], arrWorld[6]);

	m_arrFace[FACE_TOP] = XMPlaneFromPoints(arrWorld[0], arrWorld[4], arrWorld[5]);
	m_arrFace[FACE_BOT] = XMPlaneFromPoints(arrWorld[2], arrWorld[6], arrWorld[7]);
}

bool CFrustum::FrustumPointCheck(Vec3 _WorldPos)
{
	for (int i = 0; i < 6; ++i)
	{
		// a, b, c
		Vec3 vNormal = m_arrFace[i];

		// ax + by + cz + d == 0.f
		if (vNormal.Dot(_WorldPos) + m_arrFace[i].w > 0.f)
			return false;
	}

	return true;
}

bool CFrustum::FrustumSphereCheck(Vec3 _WorldPos, float _Radius)
{
	for (int i = 0; i < 6; ++i)
	{
		// a, b, c
		Vec3 vNormal = m_arrFace[i];

		// ax + by + cz + d == 0.f
		if (vNormal.Dot(_WorldPos) + m_arrFace[i].w > _Radius)
			return false;
	}

	return true;
}