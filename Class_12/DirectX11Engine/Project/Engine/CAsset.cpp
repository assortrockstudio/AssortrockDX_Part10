#include "pch.h"
#include "CAsset.h"

CAsset::CAsset(ASSET_TYPE _Type, bool _bEngine)
	: m_Type(_Type)
	, m_RefCount(0)
	, m_bEngine(_bEngine)
{
}

CAsset::CAsset(const CAsset& _Origin)
	: CEntity(_Origin)
	, m_Key(_Origin.m_Key)
	, m_RelativePath(_Origin.m_RelativePath)
	, m_Type(_Origin.m_Type)
	, m_RefCount(0)
	, m_bEngine(false)
{
}

CAsset::~CAsset()
{
}
