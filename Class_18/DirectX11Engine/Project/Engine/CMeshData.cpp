#include "pch.h"
#include "CMeshData.h"

#include "CPathMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CFBXLoader.h"

CMeshData::CMeshData(bool _Engine)
	: CAsset(ASSET_TYPE::MESHDATA, _Engine)
{
}

CMeshData::~CMeshData()
{
}

CGameObject* CMeshData::Instantiate()
{
	CGameObject* pNewObj = new CGameObject;
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRender);

	pNewObj->MeshRender()->SetMesh(m_pMesh);

	for (UINT i = 0; i < m_vecMtrl.size(); ++i)
	{
		pNewObj->MeshRender()->SetMaterial(m_vecMtrl[i], i);
	}

	return pNewObj;
}

CMeshData* CMeshData::LoadFromFBX(const wstring& _RelativePath)
{
	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
	strFullPath += _RelativePath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	// 메쉬 가져오기
	CMesh* pMesh = nullptr;
	pMesh = CMesh::CreateFromContainer(loader);

	// AssetMgr 에 메쉬 등록
	if (nullptr != pMesh)
	{
		wstring strMeshKey = L"mesh\\";
		strMeshKey += path(strFullPath).stem();
		strMeshKey += L".mesh";
		CAssetMgr::GetInst()->AddAsset<CMesh>(strMeshKey, pMesh);

		// 메시를 실제 파일로 저장
		pMesh->Save(strMeshKey);
	}

	vector<Ptr<CMaterial>> vecMtrl;

	// 메테리얼 가져오기
	for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
	{
		// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
		Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(loader.GetContainer(0).vecMtrl[i].strMtrlName);
		assert(pMtrl.Get());

		vecMtrl.push_back(pMtrl);
	}

	CMeshData* pMeshData = new CMeshData(true);
	pMeshData->m_pMesh = pMesh;
	pMeshData->m_vecMtrl = vecMtrl;

	return pMeshData;
}


int CMeshData::Save(const wstring&)
{
	return 0;
}

int CMeshData::Load(const wstring& _strFilePath)
{
	return 0;
}

