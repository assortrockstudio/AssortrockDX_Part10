#include "pch.h"
#include "CLandScape.h"

#include "CAssetMgr.h"
#include "CTexture.h"

void CLandScape::init()
{
	// LandScape 전용 Mesh 생성
	CreateMesh();

	// LandScape 전용 재질 참조
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl");
	SetMaterial(pMtrl);

	pMtrl->SetScalarParam(FLOAT_0, 1.f);
	pMtrl->GetShader()->AddScalarParam("TessFactor", FLOAT_0);

	// LandScape 전용 컴퓨트 쉐이더 제작
	CreateComputeShader();

	// BrushTexture 추가	
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\TX_GlowScene_2.png"));
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\TX_HitFlash_0.png"));
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\TX_HitFlash02.png"));
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\TX_Twirl02.png"));
	AddBrushTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\FX_Flare.png"));
	m_BrushIdx = 0;
}

void CLandScape::SetFace(UINT _X, UINT _Z)
{
	m_FaceX = _X;
	m_FaceZ = _Z;

	// 변경된 Face 개수로 새로 제작하고, 세팅
	CreateMesh();

	// LandScape 전용 재질 참조
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl");
	SetMaterial(pMtrl);
}

void CLandScape::CreateMesh()
{
	Vtx v;
	vector<Vtx> vecVtx;

	for (int Row = 0; Row < m_FaceZ + 1; ++Row)
	{
		for (int Col = 0; Col < m_FaceX + 1; ++Col)
		{
			v.vPos = Vec3((float)Col, 0.f, (float)Row);
			v.vUV = Vec2((float)Col, (float)m_FaceZ - Row);

			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}


	vector<UINT> vecIdx;

	for (int Row = 0; Row < m_FaceZ; ++Row)
	{
		for (int Col = 0; Col < m_FaceX; ++Col)
		{
			// 0
			// | \
			// 2--1
			vecIdx.push_back((m_FaceX + 1) * (Row + 1) + Col);
			vecIdx.push_back((m_FaceX + 1) * (Row)     + Col + 1);
			vecIdx.push_back((m_FaceX + 1) * (Row)	   + Col);

			// 1--2
			//  \ |
			//    0
			vecIdx.push_back((m_FaceX + 1) * (Row)     + Col + 1);
			vecIdx.push_back((m_FaceX + 1) * (Row + 1) + Col);
			vecIdx.push_back((m_FaceX + 1) * (Row + 1) + Col + 1);
		}
	}

	Ptr<CMesh> pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	SetMesh(pMesh);
}

void CLandScape::CreateComputeShader()
{
	// HeightMapCS 가 있으면 찾아오고 없으면 컴파일해서 등록한다.
	m_HeightMapCS = (CHeightMapCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"HeightMapCS").Get();
	if (nullptr == m_HeightMapCS)
	{
		m_HeightMapCS = new CHeightMapCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"HeightMapCS", m_HeightMapCS.Get());
	}

	// RaycastCS 생성
	m_RaycastCS = (CRaycastCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"RaycastCS").Get();
	if (nullptr == m_RaycastCS)
	{
		m_RaycastCS = new CRaycastCS;
		CAssetMgr::GetInst()->AddAsset<CComputeShader>(L"RaycastCS", m_RaycastCS.Get());
	}
}

void CLandScape::CreateHeightMap(UINT _Width, UINT _Height)
{
	m_IsHeightMapCreated = true;

	m_HeightMap = CAssetMgr::GetInst()->CreateTexture(L"LandScapeHeightMap", _Width, _Height
										   , DXGI_FORMAT_R32_FLOAT
									       , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

}