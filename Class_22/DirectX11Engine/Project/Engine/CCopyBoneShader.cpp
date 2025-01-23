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
	// ����ȭ���� ����
	m_SrcBuffer->Binding_CS_SRV(18);  // t18
	m_DestBuffer->Binding_CS_UAV(0);  // u0

	return S_OK;
}

void CCopyBoneShader::CalculateGroupNum()
{
	// �׷� �� ���
	int iBoneCount = m_Const.iArr[0];

	m_GroupX = iBoneCount / m_ThreadPerGroupX + 1;
	m_GroupY = 1;
	m_GroupZ = 1;
}

void CCopyBoneShader::Clear()
{
	// ������ ����ȭ���� Ŭ����	
	m_SrcBuffer->Clear_CS_SRV();
	m_DestBuffer->Clear_CS_UAV();

	m_SrcBuffer = nullptr;
	m_DestBuffer = nullptr;
}
