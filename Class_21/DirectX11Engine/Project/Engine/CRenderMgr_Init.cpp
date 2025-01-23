#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "assets.h"

#include "CMRT.h"


void CRenderMgr::init()
{
	// RenderTarget 을 복사받을 수 있는 텍스쳐를 생성
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	m_RenderTargetCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetCopyTex"
		, (UINT)vRenderResol.x, (UINT)vRenderResol.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);


	// MRT 생성
	CreateMRT();
}

void CRenderMgr::CreateMRT()
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	CMRT* pMRT = nullptr;

	// =============
	// SwapChain MRT
	// =============	
	pMRT = m_MRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;

	Ptr<CTexture> RTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");

	// DepthStencil Texture 를 생성한다.
	Ptr<CTexture> DSTex = CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex"
								, (UINT)vResolution.x, (UINT)vResolution.y
								, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	pMRT->Create(&RTTex, 1, DSTex);

	
	// ============
	// Deferred MRT
	// ============
	{
		pMRT = m_MRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;

		Ptr<CTexture> arrRTTex[8] =
		{
			// 0. ColorTarget ( 물체의 색상)
			CAssetMgr::GetInst()->CreateTexture(L"ColorTargetTex"
								, (UINT)vResolution.x, (UINT)vResolution.y
								, DXGI_FORMAT_R8G8B8A8_UNORM
								, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),

			// 1. NormalTarget ( 물체 표면의 노말정보)
			CAssetMgr::GetInst()->CreateTexture(L"NormalTargetTex"
								, (UINT)vResolution.x, (UINT)vResolution.y
								, DXGI_FORMAT_R32G32B32A32_FLOAT
								, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),

			// 2. PositionTarget ( 물체의 위치정보)
			CAssetMgr::GetInst()->CreateTexture(L"PositionTargetTex"
								, (UINT)vResolution.x, (UINT)vResolution.y
								, DXGI_FORMAT_R32G32B32A32_FLOAT
								, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),

			// 3. EmissiveTarget ( 발광 정보 )
			CAssetMgr::GetInst()->CreateTexture(L"EmissiveTargetTex"
								, (UINT)vResolution.x, (UINT)vResolution.y
								, DXGI_FORMAT_R32G32B32A32_FLOAT
								, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
	
			// 4. DataTarget ( 커스텀 데이터 )
			CAssetMgr::GetInst()->CreateTexture(L"DataTargetTex"
								, (UINT)vResolution.x, (UINT)vResolution.y
								, DXGI_FORMAT_R32G32B32A32_FLOAT
								, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
		};
		
		pMRT->Create(arrRTTex, 5, DSTex);		
	}

	// ==========
	// Decal MRT
	// ==========
	{
		pMRT = m_MRT[(UINT)MRT_TYPE::DECAL] = new CMRT;

		Ptr<CTexture> arrRTTex[8] =
		{
			// 0. ColorTarget ( 물체의 색상)
			CAssetMgr::GetInst()->FindAsset<CTexture>(L"ColorTargetTex"),
			
			// 1. NormalTarget ( 물체 표면의 노말정보)
			CAssetMgr::GetInst()->FindAsset<CTexture>(L"EmissiveTargetTex"),
		};

		pMRT->Create(arrRTTex, 2, nullptr);
	}

	// =========
	// Light MRT
	// =========
	{
		pMRT = m_MRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;

		Ptr<CTexture> arrRTTex[8] =
		{
			// 1. DiffuseTarget ( 물체에 빛이 부딪힌 세기 (난반사 세기) )
			CAssetMgr::GetInst()->CreateTexture(L"DiffuseTargetTex"
								, (UINT)vResolution.x, (UINT)vResolution.y
								, DXGI_FORMAT_R32G32B32A32_FLOAT
								, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),

			// 2. SpecularTarget ( 물체에서 반사된 빛이 카메라로 들어오는 세기 )
			CAssetMgr::GetInst()->CreateTexture(L"SpecularTargetTex"
								, (UINT)vResolution.x, (UINT)vResolution.y
								, DXGI_FORMAT_R32G32B32A32_FLOAT
								, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),			
		};

		pMRT->Create(arrRTTex, 2, nullptr);
	}
}

void CRenderMgr::ClearMRT()
{
	m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearTarget();
	m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearDepthStencil();

	m_MRT[(UINT)MRT_TYPE::DEFERRED]->ClearTarget();
	m_MRT[(UINT)MRT_TYPE::LIGHT]->ClearTarget();
}