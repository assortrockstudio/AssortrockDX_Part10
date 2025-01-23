#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"
#include "CTransform.h"

PREFAB_SAVE_FUNC CPrefab::SAVE_FUNC = nullptr;
PREFAB_LOAD_FUNC CPrefab::LOAD_FUNC = nullptr;

CPrefab::CPrefab(bool _bEngine)
	: CAsset(ASSET_TYPE::PREFAB, _bEngine)
	, m_ProtoObj(nullptr)
{
}

CPrefab::CPrefab(CGameObject* _Proto)
	: CAsset(ASSET_TYPE::PREFAB, false)
	, m_ProtoObj(_Proto)
{
	m_ProtoObj->Transform()->SetRelativePos(0.f, 0.f, 0.f);
}

CPrefab::CPrefab(const CPrefab& _Other)
	: CAsset(_Other)
	, m_ProtoObj(nullptr)
{
	m_ProtoObj = _Other.m_ProtoObj->Clone();
	m_ProtoObj->Transform()->SetRelativePos(0.f, 0.f, 0.f);
}

CPrefab::~CPrefab()
{
	if (nullptr != m_ProtoObj)
		delete m_ProtoObj;
}

CGameObject* CPrefab::Instantiate()
{
	return m_ProtoObj->Clone();
}


int CPrefab::Save(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"wb");
	assert(File);

	SAVE_FUNC(m_ProtoObj, File);

	fclose(File);

	return S_OK;
}

int CPrefab::Load(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"rb");
	assert(File);

	m_ProtoObj = LOAD_FUNC(File);

	fclose(File);

	return S_OK;
}

