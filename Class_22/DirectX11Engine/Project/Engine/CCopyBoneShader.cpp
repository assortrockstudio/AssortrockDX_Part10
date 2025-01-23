#include "pch.h"
#include "CCopyBoneShader.h"

#include "CStructuredBuffer.h"

CCopyBoneShader::CCopyBoneShader()
	: CComputeShader(256, 1, 1, L"shader\\copybone.fx", "CS_CopyBoneMatrix")
	, m_SrcBuffer(nullptr)
	, m_DestBuffer(nullptr)
{	
}

CCopyBoneShader::~CCopyBoneShader()
{

}

int CCopyBoneShader::Binding()
{
	// 구조화버퍼 전달
	m_SrcBuffer->Binding_CS_SRV(18);  // t18
	m_DestBuffer->Binding_CS_UAV(0);  // u0

	return S_OK;
}

void CCopyBoneShader::CalculateGroupNum()
{
	// 그룹 수 계산
	int iBoneCount = m_Const.iArr[0];

	m_GroupX = iBoneCount / m_ThreadPerGroupX + 1;
	m_GroupY = 1;
	m_GroupZ = 1;
}

void CCopyBoneShader::Clear()
{
	// 전달한 구조화버퍼 클리어	
	m_SrcBuffer->Clear_CS_SRV();
	m_DestBuffer->Clear_CS_UAV();

	m_SrcBuffer = nullptr;
	m_DestBuffer = nullptr;
}
