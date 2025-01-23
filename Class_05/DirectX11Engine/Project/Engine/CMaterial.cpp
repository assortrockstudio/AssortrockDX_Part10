#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CGraphicShader.h"

#include "CLevelMgr.h"
#include "CLevel.h"



CMaterial::CMaterial(bool _bEngine)
	: CAsset(ASSET_TYPE::MATERIAL, _bEngine)
	, m_bDynamic(false)
{
	
}

CMaterial::~CMaterial()
{

}

void CMaterial::SetShader(Ptr<CGraphicShader> _Shader)
{
	bool bSave = false;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (m_Shader != _Shader && !IsEngineAsset() && !m_bDynamic && pCurLevel->GetState() == LEVEL_STATE::STOP )
	{
		bSave = true;
	}	

	m_Shader = _Shader;
	
	if (bSave)
	{
		Save(CPathMgr::GetInst()->GetContentPath() + GetRelativePath());
	}	
}


void CMaterial::SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _Tex)
{
	m_arrTex[_Param] = _Tex;
}

void* CMaterial::GetScalarParam(SCALAR_PARAM _Param)
{
	switch (_Param)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		return &m_Const.iArr[_Param];
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		return &m_Const.fArr[_Param - FLOAT_0];
		break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		return &m_Const.v2Arr[_Param - VEC2_0];
		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		return &m_Const.v4Arr[_Param - VEC4_0];
		break;
	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		return &m_Const.matArr[_Param - MAT_0];
		break;	
	}

	return nullptr;
}

Ptr<CMaterial> CMaterial::GetDynamicMaterial()
{
	Ptr<CMaterial> pDynamicMtrl = Clone();
	pDynamicMtrl->m_bDynamic = true;
	return pDynamicMtrl;
}

void CMaterial::Binding()
{
	// 텍스쳐 바인딩
	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		if (nullptr == m_arrTex[i])
		{
			CTexture::Clear(i);
			m_Const.btex[i] = false;
			continue;
		}			

		m_arrTex[i]->Binding(i);
		m_Const.btex[i] = true;
	}


	// 상수 데이터 바인딩
	if (nullptr != m_Shader.Get())
		m_Shader->Binding();

	CConstBuffer* pMtrlCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pMtrlCB->SetData(&m_Const);
	pMtrlCB->Binding();
}

int CMaterial::Save(const wstring& _FilePath)
{
	// 동적 재질 및 Engine 용 재질은 저장될 수 없다.
	assert(!m_bDynamic && !IsEngineAsset());
	
	SetRelativePath(CPathMgr::GetInst()->GetRelativePath(_FilePath));

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

	// 상수 데이터 저장
	fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);
		
	// 에셋의 참조정보 저장
	SaveAssetRef(m_Shader, pFile);

	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		SaveAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);

	return S_OK;
}

int CMaterial::Load(const wstring& _FilePath)
{
	// 동적 재질 및 Engine 용 재질은 파일로부터 불러올 수 없다.
	assert(!m_bDynamic && !IsEngineAsset());

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	if (nullptr == pFile)
	{
		return E_FAIL;
	}

	// 상수 데이터 저장
	fread(&m_Const, sizeof(tMtrlConst), 1, pFile);

	// 에셋의 참조정보 로드
	LoadAssetRef(m_Shader, pFile);

	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		LoadAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);

	return S_OK;
}