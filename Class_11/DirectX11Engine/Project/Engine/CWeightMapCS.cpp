#include "pch.h"
#include "CWeightMapCS.h"

#include "CStructuredBuffer.h"

CWeightMapCS::CWeightMapCS()
	: CComputeShader(32, 32, 1, L"shader\\weightmap.fx", "CS_WeightMap")
	, m_WeightMap(nullptr)
	, m_RaycastOut(nullptr)
	, m_WeightIdx(0)
	, m_WeightMapWidth(0)
	, m_WeightMapHeight(0)
{
}

CWeightMapCS::~CWeightMapCS()
{
}

int CWeightMapCS::Binding()
{
	if (nullptr == m_WeightMap || nullptr == m_BrushTex || nullptr == m_RaycastOut
		|| 0 == m_WeightMapWidth || 0 == m_WeightMapHeight)
		return E_FAIL;

	m_Const.iArr[0] = m_WeightMapWidth;
	m_Const.iArr[1] = m_WeightMapHeight;
	m_Const.iArr[2] = m_WeightIdx;
	m_Const.v2Arr[0] = m_BrushScale;

	m_BrushTex->Binding_CS_SRV(0);
	m_WeightMap->Binding_CS_UAV(0);
	m_RaycastOut->Binding_CS_SRV(20);

	return S_OK;
}

void CWeightMapCS::CalculateGroupNum()
{
	m_GroupX = m_WeightMapWidth / m_ThreadPerGroupX;
	m_GroupY = m_WeightMapHeight / m_ThreadPerGroupY;

	if (0 < (UINT)m_WeightMapWidth % m_ThreadPerGroupX)
		m_GroupX += 1;

	if (0 < (UINT)m_WeightMapHeight % m_ThreadPerGroupY)
		m_GroupY += 1;
}

void CWeightMapCS::Clear()
{
	m_BrushTex->Clear_CS_SRV(0);
	m_WeightMap->Clear_UAV();
	m_RaycastOut->Clear_SRV();
}