#include "pch.h"
#include "CTileMap.h"

#include "CAssetMgr.h"
#include "components.h"

#include "CStructuredBuffer.h"
#include "CTexture.h"

#include "CDevice.h"


CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_Row(1)
	, m_Col(1)
	, m_AtlasMaxRow(0)
	, m_AtlasMaxCol(0)
	, m_TileEachSize(Vec2(32.f, 32.f))
	, m_CaptureTexMaxResolution(Vec2(2048, 2048))
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));
	
	m_TileBuffer = new CStructuredBuffer;
}

CTileMap::CTileMap(const CTileMap& _Other)
	: CRenderComponent(_Other)
	, m_Row(_Other.m_Row)
	, m_Col(_Other.m_Col)
	, m_TileEachSize(_Other.m_TileEachSize)
	, m_Atlas(_Other.m_Atlas)
	, m_AtlasMaxRow(_Other.m_AtlasMaxRow)
	, m_AtlasMaxCol(_Other.m_AtlasMaxCol)
	, m_AtlasTileEachSize(_Other.m_AtlasTileEachSize)
	, m_AtlasResolution(_Other.m_AtlasResolution)
	, m_vecTileInfo(_Other.m_vecTileInfo)
	, m_TileBuffer(nullptr)
{
	//m_TileBuffer = new CStructuredBuffer(*_Other.m_TileBuffer);

	m_TileBuffer = new CStructuredBuffer;
	m_TileBuffer->Create(sizeof(tTileInfo), m_vecTileInfo.size(), SB_TYPE::SRV_ONLY, true, m_vecTileInfo.data());	
}

CTileMap::~CTileMap()
{
	delete m_TileBuffer;
}

void CTileMap::begin()
{
	SetRowCol(m_Row, m_Col);
}

void CTileMap::finaltick()
{

}

void CTileMap::render()
{
	// 위치정보 
	Transform()->Binding();

	// 재질정보
	GetMaterial()->SetTexParam(TEX_0, m_Atlas);
	GetMaterial()->SetScalarParam(VEC2_0, m_AtlasResolution);
	GetMaterial()->SetScalarParam(VEC2_1, m_AtlasTileEachSize);
	GetMaterial()->SetScalarParam(VEC2_2, Vec2(m_Col, m_Row));
	GetMaterial()->SetScalarParam(INT_0, m_AtlasMaxRow);
	GetMaterial()->SetScalarParam(INT_1, m_AtlasMaxCol);
	GetMaterial()->SetScalarParam(INT_2, 0);
	GetMaterial()->SetScalarParam(INT_3, 0); // CaptureMode 가 아니다.
	GetMaterial()->Binding();

	// 타일정보 바인딩
	if (m_TileBuffer->GetElementCount() < m_vecTileInfo.size())
		m_TileBuffer->Create(sizeof(tTileInfo), m_vecTileInfo.size(), SB_TYPE::SRV_ONLY, true, m_vecTileInfo.data());
	else
		m_TileBuffer->SetData(m_vecTileInfo.data(), m_vecTileInfo.size());

	m_TileBuffer->Binding(20);

	// 메시 바인딩 및 렌더링
	GetMesh()->render();

	if (m_EditorCapture)
	{
		CaptureRender();
	}	
}

void CTileMap::CaptureRender()
{
	// 1. 현재 타일의 상황을 캡쳐할 텍스쳐(그림을 그릴 목적지, 렌더타겟) 를 체크
	CheckCaptureTexture();

	// 2. 렌더타겟이 일시적으로 교체되어야 함
	// 기존에 세팅되어있단 타겟이 누군지 알아냄
	ComPtr<ID3D11RenderTargetView> pRTV = nullptr;
	ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	CONTEXT->OMGetRenderTargets(1, pRTV.GetAddressOf(), pDSV.GetAddressOf());

	// CaptureTexture 로 렌더타겟을 변경처리함
	CONTEXT->OMSetRenderTargets(1, m_CaptureTex->GetRTV().GetAddressOf(), nullptr);

	// ViewPort 설정
	// 텍스쳐 크기 영역을 설정
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_CaptureTex->GetWidth();
	viewport.Height = m_CaptureTex->GetHeight();
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	CONTEXT->RSSetViewports(1, &viewport);

	// 3. 타일을 교체된 렌더타겟으로 렌더링 한다.	
	GetMaterial()->SetScalarParam(INT_3, 1); // CaptureMode 이다
	GetMaterial()->Binding();
	GetMesh()->render();

	// 4. 캡처 렌더링이 끝난 후에는 다시 원래 렌더타겟과 깊이텍스쳐로 되돌려줌
	CONTEXT->OMSetRenderTargets(1, pRTV.GetAddressOf(), pDSV.Get());

	// ViewPort 설정
	// 윈도우 화면에 보여질 영역을 설정	
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = CDevice::GetInst()->GetRenderResolution().x;
	viewport.Height = CDevice::GetInst()->GetRenderResolution().y;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	CONTEXT->RSSetViewports(1, &viewport);
}

void CTileMap::CheckCaptureTexture()
{
	// 1. 텍스쳐가 존재하는지
	if (nullptr == m_CaptureTex)
	{
		Vec2 vResolution = Vec2((float)m_Col * m_TileEachSize.x, (float)m_Row * m_TileEachSize.y);

		if (m_CaptureTexMaxResolution.x < vResolution.x)
			vResolution.x = m_CaptureTexMaxResolution.x;
		if (m_CaptureTexMaxResolution.y < vResolution.y)
			vResolution.y = m_CaptureTexMaxResolution.y;

		m_CaptureTex = new CTexture(true);
		m_CaptureTex->Create((UINT)vResolution.x, (UINT)vResolution.y
							, DXGI_FORMAT_R8G8B8A8_UNORM
							, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
	}
	else
	{
		// Capture Texture 가 생성되어있지만, 예상되는 해상도 보다 작은경우, 새로 만든다.
		Vec2 vResolution = Vec2((float)m_Col * m_TileEachSize.x, (float)m_Row * m_TileEachSize.y);

		if (m_CaptureTexMaxResolution.x < vResolution.x)
			vResolution.x = m_CaptureTexMaxResolution.x;
		if (m_CaptureTexMaxResolution.y < vResolution.y)
			vResolution.y = m_CaptureTexMaxResolution.y;

		if (m_CaptureTex->GetWidth() < vResolution.x || m_CaptureTex->GetHeight() < vResolution.y)
		{			
			m_CaptureTex = new CTexture(true);
			m_CaptureTex->Create((UINT)vResolution.x, (UINT)vResolution.y
				, DXGI_FORMAT_R8G8B8A8_UNORM
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);			
		}
	}
}

void CTileMap::SetRowCol(UINT _Row, UINT _Col)
{
	m_Row = _Row;
	m_Col = _Col;

	Transform()->SetRelativeScale(Vec3(m_TileEachSize.x * m_Col, m_TileEachSize.y * m_Row, 1.f));

	m_vecTileInfo.clear();
	m_vecTileInfo.resize(m_Row * m_Col);

	for (int i = 0; i < m_vecTileInfo.size(); ++i)
	{
		m_vecTileInfo[i].ImgIdx = i % (m_AtlasMaxRow * m_AtlasMaxCol);
		//m_vecTileInfo[i].ImgIdx = 7;
	}
}

void CTileMap::SetTileEachSize(Vec2 _vSize)
{
	m_TileEachSize = _vSize;
	Transform()->SetRelativeScale(Vec3(m_TileEachSize.x * m_Col, m_TileEachSize.y * m_Row, 1.f));
}

void CTileMap::SetImgIndex(int _Row, int _Col, int _imgidx)
{	
	if(_Row < -1 || m_Row <= _Row || _Col < -1 || m_Col <= _Col)
	{
		return;
	}
	int idx = _Row * m_Col + _Col;
	m_vecTileInfo[idx].ImgIdx = _imgidx;
}

void CTileMap::SetAtlasTexture(Ptr<CTexture> _Tex)
{
	m_Atlas = _Tex;

	if (nullptr != m_Atlas)
	{
		m_AtlasResolution = Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
	}	
}

void CTileMap::SetAtlasTileSize(Vec2 _TileSize)
{
	assert(m_Atlas.Get());	

	// 아틀라스 텍스처 안에서 타일 1개가 차지하는 크기
	m_AtlasTileEachSize = _TileSize;

	// 아틀라스 텍스쳐 해상도를 타일1개 사이즈로 나누어서 
	// 아틀라트 텍스쳐에 타일이 몇행 몇열 존재하는지 확인
	m_AtlasMaxCol = m_Atlas->GetWidth() / m_AtlasTileEachSize.x;
	m_AtlasMaxRow = m_Atlas->GetHeight() / m_AtlasTileEachSize.y;
}



void CTileMap::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	fwrite(&m_Row, sizeof(UINT), 1, _File);
	fwrite(&m_Col, sizeof(UINT), 1, _File);
	fwrite(&m_TileEachSize, sizeof(Vec2), 1, _File);
	
	SaveAssetRef(m_Atlas, _File);
	fwrite(&m_AtlasTileEachSize, sizeof(Vec2), 1, _File);


	size_t i = m_vecTileInfo.size();
	fwrite(&i, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		fwrite(&m_vecTileInfo[i], sizeof(tTileInfo), 1, _File);
	}
}

void CTileMap::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_Row, sizeof(UINT), 1, _File);
	fread(&m_Col, sizeof(UINT), 1, _File);
	fread(&m_TileEachSize, sizeof(Vec2), 1, _File);
	LoadAssetRef(m_Atlas, _File);
	fread(&m_AtlasTileEachSize, sizeof(Vec2), 1, _File);

	size_t size = 0;
	fread(&size, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < size; ++i)
	{
		tTileInfo info = {};
		fread(&info, sizeof(tTileInfo), 1, _File);
		m_vecTileInfo.push_back(info);
	}

	// 파일에서 읽은 값으로 TileMap 에 정보 세팅
	SetAtlasTexture(m_Atlas);
	SetAtlasTileSize(m_AtlasTileEachSize);
	SetRowCol(m_Row, m_Col);
}