#include "pch.h"
#include "CComputeShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CComputeShader::CComputeShader(UINT _ThreadPerGroupX, UINT _ThreadPerGroupY, UINT _ThreadPerGroupZ
, const wstring& _RelativePath, const string& _FuncName)
	: CShader(ASSET_TYPE::COMPUTE_SHADER)
	, m_ThreadPerGroupX(_ThreadPerGroupX)
	, m_ThreadPerGroupY(_ThreadPerGroupY)
	, m_ThreadPerGroupZ(_ThreadPerGroupZ)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();

	if (FAILED(CreateComputeShader(strPath + _RelativePath, _FuncName)))
	{
		assert(nullptr);
	}
}

CComputeShader::~CComputeShader()
{
}

int CComputeShader::CreateComputeShader(const wstring& _strFilePath, const string& _CSFuncName)
{
	if (FAILED(D3DCompileFromFile(_strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _CSFuncName.c_str(), "cs_5_0", D3DCOMPILE_DEBUG, 0
		, m_CSBlob.GetAddressOf()
		, m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			MessageBoxA(nullptr
				, (char*)m_ErrBlob->GetBufferPointer()
				, "컴퓨트 쉐이더 컴파일 오류", MB_OK);
		}
		else
		{
			MessageBox(nullptr
				, L"파일을 찾을 수 없습니다."
				, L"컴퓨트 쉐이더 컴파일 오류", MB_OK);
		}

		return E_FAIL;
	}
		

	// ComputeShader 객체 생쇼ㅓㅇ
	if (FAILED(DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer()
		, m_CSBlob->GetBufferSize(), nullptr, m_CS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}


int CComputeShader::Execute()
{
	// 필요한 데이터를 바인딩한다.
	if (FAILED(Binding()))
	{
		return E_FAIL;
	}

	// 사용할 그룹 개수를 계산한다.
	CalculateGroupNum();

	// 상수데이터를 ComputeShader 전달
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Binding_CS();


	// ComputeShader 실행
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ); // Group Count

	Clear();

	return S_OK;
}

