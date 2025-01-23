#include "pch.h"
#include "CAssetMgr.h"

#include "CPathMgr.h"

void CAssetMgr::init()
{
	CreateDefaultMesh();

	CreateDefaultTexture();

	CreateDefaultGraphicShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();
}


void CAssetMgr::CreateDefaultMesh()
{
	Ptr<CMesh> pMesh = nullptr;

	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// =========
	// PointMesh
	// =========
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT idx = 0;

	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &idx, 1);
	AddAsset<CMesh>(L"PointMesh", pMesh);


	// =========
	// Rect Mesh
	// =========
	// 0 -- 1
	// | \  |
	// 3 -- 2
	v.vPos = Vec3(-0.5, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);

	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	// Index Buffer
	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset<CMesh>(L"RectMesh", pMesh);
		
	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset<CMesh>(L"RectMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();

	// ===========
	// Circle Mesh
	// ===========
	// 원점을 vector 에 넣는다.
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	float Radius = 0.5f;
	UINT Slice = 60;
	float AngleStep = (2 * XM_PI) / Slice;

	float Angle = 0.f;
	for (UINT i = 0; i <= Slice; ++i)
	{
		v.vPos = Vec3(cosf(Angle) * Radius, sinf(Angle) * Radius, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Angle += AngleStep;
	}

	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset<CMesh>(L"CircleMesh", pMesh);
	

	vecIdx.clear();
	for (int i = 0; i <= (int)Slice; ++i)
	{
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset<CMesh>(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();


	// ==========
	// Cube Mesh
	// ==========
	// 6개의 각 면 마다 각자의 방향벡터(노말, 탄젠트, 바이노말) 이 세팅되어야 하기 때문에
	// 면 단위로 정점 4개씩 6면, 총 24개의 정점이 필요하다.	
	Vtx arrCube[24] = {};

	// 윗면
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vec2(0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vec2(0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vec2(0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// 아랫 면	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vec2(0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vec2(0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vec2(0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// 왼쪽 면
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vec2(0.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vec2(0.f, 0.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vec2(0.f, 0.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// 오른쪽 면
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vec2(0.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vec2(0.f, 0.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vec2(0.f, 0.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// 뒷 면
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vec2(0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vec2(0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vec2(0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// 앞 면
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vec2(0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vec2(0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vec2(0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CubeMesh", pMesh);
	vecIdx.clear();

	UINT arrCubeIdx[] = { 0,1,2,3,0,7,6,1,2,5,4,3,0,7,4,5,6 };

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, arrCubeIdx, sizeof(arrCubeIdx) / sizeof(UINT));
	AddAsset(L"CubeMesh_Debug", pMesh);
	vecIdx.clear();

	// ============
	// Sphere Mesh
	// ============
	float fRadius = 0.5f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// Body
	UINT iStackCount = 40; // 가로 분할 개수
	UINT iSliceCount = 40; // 세로 분할 개수

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));

			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vNormal.Cross(v.vTangent, v.vBinormal);
			v.vBinormal.Normalize();

			vecVtx.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"SphereMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void CAssetMgr::CreateDefaultTexture()
{
	Load<CTexture>(L"texture\\Character.png", L"texture\\Character.png");
	Load<CTexture>(L"texture\\Fighter.bmp", L"texture\\Fighter.bmp");	
}


void CAssetMgr::CreateDefaultGraphicShader()
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	Ptr<CGraphicShader> pShader = nullptr;
	
	// ===========
	// Std2DShader
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(strPath + L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	// Paramter
	pShader->AddScalarParam("Test Parameter", SCALAR_PARAM::INT_0);
	pShader->AddScalarParam("Paper Burn Intence", SCALAR_PARAM::FLOAT_0);
	pShader->AddTexParam("Output", TEX_0);
	pShader->AddTexParam("PaperBurn Noise", TEX_1);

	AddAsset<CGraphicShader>(L"Std2DShader", pShader);


	// =======================
	// Std2D AlphaBlend Shader
	// =======================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(strPath + L"shader\\std2d.fx", "PS_Std2D_AB");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset<CGraphicShader>(L"Std2DAlphaBlendShader", pShader);

	// =============
	// TileMapShader
	// =============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(strPath + L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);	
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	// Parameter
	pShader->AddScalarParam("Temp", SCALAR_PARAM::INT_0);

	AddAsset<CGraphicShader>(L"TileMapShader", pShader);

	// ======================
	// Particle Render Shader
	// ======================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(strPath + L"shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(strPath + L"shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetRSType(RS_TYPE::CULL_NONE);	
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset<CGraphicShader>(L"ParticleRenderShader", pShader);


	// =========================
	// PostProcess Filter Shader
	// =========================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\postprocess.fx", "VS_PostProcess");	
	pShader->CreatePixelShader(strPath + L"shader\\postprocess.fx", "PS_PostProcess");
		
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);	
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset<CGraphicShader>(L"FilterShader", pShader);

	// =============================
	// PostProcess Distortion Shader
	// =============================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(strPath + L"shader\\postprocess.fx", "PS_Distortion");

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset<CGraphicShader>(L"DistortionShader", pShader);

	// ================
	// DebugShapeShader
	// ================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\debug_shape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(strPath + L"shader\\debug_shape.fx", "PS_DebugShape");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	AddAsset<CGraphicShader>(L"DebugShapeShader", pShader);


	// ============
	// Std3D Shader
	// ============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\std3d.fx", "VS_Std3D");
	pShader->CreatePixelShader(strPath + L"shader\\std3d.fx", "PS_Std3D");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	//pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);	

	pShader->AddTexParam("Output Texture", TEX_0);
	pShader->AddTexParam("Normal Texture", TEX_1);

	AddAsset<CGraphicShader>(L"Std3DShader", pShader);

	// =============
	// SkyBox Shader
	// =============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\skybox.fx", "VS_SkyBox");
	pShader->CreatePixelShader(strPath + L"shader\\skybox.fx", "PS_SkyBox");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);	
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	pShader->AddTexParam("SkyBox Texture", TEX_0);	

	AddAsset<CGraphicShader>(L"SkyBoxShader", pShader);


	// ===============
	// Deferred Shader
	// ===============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	pShader->CreatePixelShader(strPath + L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

	pShader->AddTexParam("Color", TEX_0);
	pShader->AddTexParam("Normal", TEX_1);
	pShader->AddTexParam("Specular", TEX_2);
	pShader->AddTexParam("Heightmap", TEX_3);
	pShader->AddTexParam("Emissive", TEX_4);

	AddAsset<CGraphicShader>(L"Std3DDeferredShader", pShader);


	// ==============
	// DirLightShader
	// ==============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\light.fx", "VS_DirLight");
	pShader->CreatePixelShader(strPath + L"shader\\light.fx", "PS_DirLight");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::ONE_ONE); // 빛이 누적되어서 합쳐져간다.
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHTING);
	
	AddAsset<CGraphicShader>(L"DirLightShader", pShader);

	// ================
	// PointLightShader
	// ================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\light.fx", "VS_PointLight");
	pShader->CreatePixelShader(strPath + L"shader\\light.fx", "PS_PointLight");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetBSType(BS_TYPE::ONE_ONE); // 빛이 누적되어서 합쳐져간다.
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHTING);

	AddAsset<CGraphicShader>(L"PointLightShader", pShader);

	// ===========
	// MergeShader
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\merge.fx", "VS_Merge");
	pShader->CreatePixelShader(strPath + L"shader\\merge.fx", "PS_Merge");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::DEFAULT); // 빛이 누적되어서 합쳐져간다.
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHTING);

	AddAsset<CGraphicShader>(L"MergeShader", pShader);


	// ===========
	// DecalShader
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\decal.fx", "VS_Decal");
	pShader->CreatePixelShader(strPath + L"shader\\decal.fx", "PS_Decal");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetBSType(BS_TYPE::DECAL_BLEND); 
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);

	AddAsset<CGraphicShader>(L"DecalShader", pShader);


	// ==========
	// TessShader
	// ==========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\tess.fx", "VS_Tess");
	pShader->CreateHullShader(strPath + L"shader\\tess.fx", "HS_Tess");
	pShader->CreateDomainShader(strPath + L"shader\\tess.fx", "DS_Tess");
	pShader->CreatePixelShader(strPath + L"shader\\tess.fx", "PS_Tess");

	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	AddAsset<CGraphicShader>(L"TessShader", pShader);


	// ================
	// LandScape Shader
	// ================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\landscape.fx", "VS_LandScape");
	pShader->CreateHullShader(strPath + L"shader\\landscape.fx",   "HS_LandScape");
	pShader->CreateDomainShader(strPath + L"shader\\landscape.fx", "DS_LandScape");
	pShader->CreatePixelShader(strPath + L"shader\\landscape.fx",  "PS_LandScape");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	AddAsset<CGraphicShader>(L"LandScapeShader", pShader);
}


#include "CSetColorCS.h"
#include "CParticleTickCS.h"

void CAssetMgr::CreateDefaultComputeShader()
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();

	Ptr<CComputeShader> pComputeShader = nullptr;

	// SetColor 
	pComputeShader = new CSetColorCS;	
	AddAsset<CComputeShader>(L"SetColorCS", pComputeShader);

	// ParticleTick
	pComputeShader = new CParticleTickCS;	
	AddAsset<CComputeShader>(L"ParticleCS", pComputeShader);
}

void CAssetMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// BackgroundMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"BackgroundMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// TileMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"TileMapMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// ParticleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleRenderShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// FilterMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"FilterMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"FilterShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
	
	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DistortionMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DebugShapeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);	

	// Std3DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std3DMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std3DShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// SkyBoxMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"SkyBoxMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"SkyBoxShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// DeferredMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std3DDeferredMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std3DDeferredShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);	

	// DirLightMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DirLightMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DirLightShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// PointLightMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"PointLightMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"PointLightShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// MergeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"MergeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"MergeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// DecalMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DecalMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DecalShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);	

	// TessMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"TessMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TessShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// LandScapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"LandScapeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"LandScapeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
}