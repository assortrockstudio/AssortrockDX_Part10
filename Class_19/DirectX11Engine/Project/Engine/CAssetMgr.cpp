#include "pch.h"
#include "CAssetMgr.h"

#include "CTexture.h"


CAssetMgr::CAssetMgr()
	: m_AssetChanged(false)
{

}

CAssetMgr::~CAssetMgr()
{

}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
									 , DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	pTex = new CTexture;
	pTex->Create(_Width, _Height, _Format, _BindFlag, _Usage);

	pTex->m_Key = _strKey;
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_strKey, pTex.Get()));

	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	pTex = new CTexture;
	pTex->Create(_Tex2D);

	pTex->m_Key = _strKey;
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_strKey, pTex.Get()));

	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, const vector<Ptr<CTexture>>& _vecTex)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	pTex = new CTexture;
	if (FAILED(pTex->CreateArrayTexture(_vecTex)))
	{	
		MessageBox(nullptr, L"Array 텍스쳐 생성 실패", L"텍스쳐 생성 오류", MB_OK);
		return nullptr;
	}

	pTex->m_Key = _strKey;
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_strKey, pTex.Get()));

	return pTex;
}



Ptr<CMeshData> CAssetMgr::LoadFBX(const wstring& _strPath)
{
	wstring strFileName = path(_strPath).stem();

	wstring strName = L"meshdata\\";
	strName += strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindAsset<CMeshData>(strName);

	if (nullptr != pMeshData)
		return pMeshData;

	pMeshData = CMeshData::LoadFromFBX(_strPath);
	pMeshData->SetKey(strName);
	pMeshData->SetRelativePath(strName);

	m_mapAsset[(UINT)ASSET_TYPE::MESHDATA].insert(make_pair(strName, pMeshData.Get()));

	// meshdata 를 실제파일로 저장	
	pMeshData->Save(CPathMgr::GetInst()->GetContentPath() + strName);

	return pMeshData;
}



void CAssetMgr::GetAssetNames(ASSET_TYPE _Type, vector<string>& _vecNames)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		_vecNames.push_back(string(pair.first.begin(), pair.first.end()));
	}
}

void CAssetMgr::DeleteAsset(ASSET_TYPE _Type, const wstring& _strKey)
{
	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_strKey);

	assert(iter != m_mapAsset[(UINT)_Type].end());

	m_mapAsset[(UINT)_Type].erase(iter);

	m_AssetChanged = true;
}