#include "pch.h"
#include "CLandScape.h"

#include "CStructuredBuffer.h"

#include "CRenderMgr.h"
#include "CKeyMgr.h"

#include "CTransform.h"
#include "CCamera.h"

#include "CMaterial.h"
#include "CMesh.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_FaceX(1)
	, m_FaceZ(1)
	, m_MinLevel(0)
	, m_MaxLevel(4)
	, m_MaxLevelRange(2000.f)
	, m_MinLevelRange(6000.f)
	, m_BrushIdx(-1)
	, m_BrushScale(Vec2(0.1f, 0.1f))
	, m_IsHeightMapCreated(false)
	, m_WeightMap(nullptr)
	, m_WeightWidth(0)
	, m_WeightHeight(0)
	, m_WeightIdx(1)
	, m_Mode(LANDSCAPE_MODE::SPLATING)
{
	// 가중치 맵
	m_WeightMap = new CStructuredBuffer;

	init();
	SetFrustumCheck(false);

	// Raycasting 결과를 받는 용도의 구조화버퍼
	m_RaycastOut = new CStructuredBuffer;
	m_RaycastOut->Create(sizeof(tRaycastOut), 1, SB_TYPE::SRV_UAV, true);
}

CLandScape::~CLandScape()
{
	delete m_RaycastOut;
	delete m_WeightMap;
}

void CLandScape::finaltick()
{
	// 모드 전환
	if (KEY_TAP(KEY::NUM7))
	{
		if (HEIGHTMAP == m_Mode)
			m_Mode = SPLATING;
		else if (SPLATING == m_Mode)
			m_Mode = NONE;
		else
			m_Mode = HEIGHTMAP;
	}

	if (NONE == m_Mode)
		return;

	Raycasting();

	if (HEIGHTMAP == m_Mode)
	{
		if (m_IsHeightMapCreated && KEY_PRESSED(KEY::LBTN))
		{
			// RayCasting
			if (m_Out.Success)
			{
				// 높이맵 설정
				m_HeightMapCS->SetBrushPos(m_RaycastOut);
				m_HeightMapCS->SetBrushScale(m_BrushScale);
				m_HeightMapCS->SetHeightMap(m_HeightMap);
				m_HeightMapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
				m_HeightMapCS->Execute();
			}
		}
	}

	else if (SPLATING == m_Mode)
	{
		if (KEY_PRESSED(KEY::LBTN) && m_WeightWidth != 0 && m_WeightHeight != 0)
		{
			m_WeightMapCS->SetBrushPos(m_RaycastOut);
			m_WeightMapCS->SetBrushScale(m_BrushScale);
			m_WeightMapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
			m_WeightMapCS->SetWeightMap(m_WeightMap);
			m_WeightMapCS->SetWeightIdx(m_WeightIdx);
			m_WeightMapCS->SetWeightMapWidthHeight(m_WeightWidth, m_WeightHeight);

			m_WeightMapCS->Execute();
		}
	}
		

	if (KEY_TAP(KEY::NUM8))
	{
		++m_BrushIdx;
		if (m_vecBrush.size() <= m_BrushIdx)
			m_BrushIdx = 0;
	}
}

void CLandScape::render()
{
	Binding();

	// WeightMap
	m_WeightMap->Binding(20);
	
	GetMesh()->render();

	// WeightMap
	m_WeightMap->Clear_SRV();
}

void CLandScape::Binding()
{
	// 위치 정보
	Transform()->Binding();

	//GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

	// 재질 정보	
	CCamera* pCam = CRenderMgr::GetInst()->GetFOVCamera();

	GetMaterial()->SetScalarParam(INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(INT_1, m_FaceZ);
	GetMaterial()->SetScalarParam(INT_2, m_Mode);
	GetMaterial()->SetScalarParam(INT_3, (int)m_ColorTex->GetArraySize());

	GetMaterial()->SetScalarParam(VEC4_0, Vec4(m_MinLevel, m_MaxLevel, m_MinLevelRange, m_MaxLevelRange));
	GetMaterial()->SetScalarParam(VEC4_1, pCam->Transform()->GetWorldPos());
	GetMaterial()->SetTexParam(TEX_0, m_HeightMap);
	GetMaterial()->SetTexParam(TEX_ARR_0, m_ColorTex);
	GetMaterial()->SetTexParam(TEX_ARR_1, m_NormalTex);

	// Brush 정보
	GetMaterial()->SetTexParam(TEX_1, m_vecBrush[m_BrushIdx]);
	GetMaterial()->SetScalarParam(VEC2_0, m_BrushScale);
	GetMaterial()->SetScalarParam(VEC2_1, m_Out.Location);
	GetMaterial()->SetScalarParam(VEC2_2, Vec2(m_WeightWidth, m_WeightHeight));
	
	GetMaterial()->Binding();
}

int CLandScape::Raycasting()
{
	// 현재 시점 카메라 가져오기
	CCamera* pCam = CRenderMgr::GetInst()->GetFOVCamera();
	if (nullptr == pCam)
		return false;

	// 구조화버퍼 클리어
	m_Out = {};
	m_Out.Distance = 0xffffffff;
	m_RaycastOut->SetData(&m_Out, 1);

	// 카메라가 시점에서 마우스를 향하는 Ray 정보를 가져옴
	tRay ray = pCam->GetRay();

	// LandScape 의 WorldInv 행렬 가져옴
	const Matrix& matWorldInv = Transform()->GetWorldMatInv();

	// 월드 기준 Ray 정보를 LandScape 의 Local 공간으로 데려감
	ray.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	ray.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	ray.vDir.Normalize();

	// Raycast 컴퓨트 쉐이더에 필요한 데이터 전달
	m_RaycastCS->SetRayInfo(ray);
	m_RaycastCS->SetFace(m_FaceX, m_FaceZ);
	m_RaycastCS->SetOutBuffer(m_RaycastOut);
	m_RaycastCS->SetHeightMap(m_HeightMap);

	// 컴퓨트쉐이더 실행
	m_RaycastCS->Execute();

	// 결과 확인
	m_RaycastOut->GetData(&m_Out, 1);

	return m_Out.Success;
}

