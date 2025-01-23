#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

#include "CAssetMgr.h"

CDevice::CDevice()
	: m_hMainWnd(nullptr)
	, m_Sampler{}
	, m_CB{}
{
}

CDevice::~CDevice()
{	
	Safe_Del_Array(m_CB);
}

int CDevice::init(HWND _hWnd, Vec2 _Resolution)
{
	m_hMainWnd = _hWnd;
	m_RenderResolution = _Resolution;

	// Global Data 세팅
	g_GlobalData.vResolution = m_RenderResolution;


	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
		, nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION
		, m_Device.GetAddressOf(), &level, m_Context.GetAddressOf())))
	{
		return E_FAIL;
	}

	// SwapChain
	if (FAILED(CreateSwapChain()))
	{
		return E_FAIL;
	}

	// View
	if (FAILED(CreateView()))
	{
		return E_FAIL;
	}
	
	

	// 필요한 상수버퍼 생성
	if (FAILED(CreateConstBuffer()))
	{
		return E_FAIL;
	}

	// 필요한 샘플러 생성
	if (FAILED(CreateSamplerState()))
	{
		return E_FAIL;
	}	

	// Rasterizer State 생성
	if (FAILED(CreateRasterizerState()))
	{
		return E_FAIL;
	}

	// DepthStencilState 생성
	if (FAILED(CreateDepthStencilState()))
	{
		return E_FAIL;
	}

	// BlendState 생성
	if (FAILED(CreateBlendState()))
	{
		return E_FAIL;
	}

	return S_OK;
}


int CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC Desc = {};

	// SwapChain 출력 윈도우
	Desc.OutputWindow = m_hMainWnd;
	Desc.Windowed = true;
	
	// RenderTarget 텍스쳐의 해상도 설정
	Desc.BufferCount = 1;
	Desc.BufferDesc.Width = m_RenderResolution.x;
	Desc.BufferDesc.Height = m_RenderResolution.y;
	Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	Desc.BufferDesc.RefreshRate.Denominator = 1;
	Desc.BufferDesc.RefreshRate.Numerator = 60;
	Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	Desc.Flags = 0;
	
	ComPtr<IDXGIDevice>		pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter>	pAdapter = nullptr;
	ComPtr<IDXGIFactory>	pFactory = nullptr;

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());
			
	// SwapChain 생성
	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &Desc, m_SwapChain.GetAddressOf())))
		return E_FAIL;

	return S_OK;
}

int CDevice::CreateView()
{
	// RenderTarget View 만들기
	// 1. RenderTarget 용 텍스쳐를 SwapChain 으로부터 가져온다.
	ComPtr<ID3D11Texture2D> Tex2D = nullptr;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)Tex2D.GetAddressOf());

	// 2. CTexture 로 변환한다.
	CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", Tex2D);	
			
	
	return S_OK;
}

#include "CAnim2D.h"

int CDevice::CreateConstBuffer()
{
	m_CB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
	m_CB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), CB_TYPE::TRANSFORM);

	m_CB[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer;
	m_CB[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(tMtrlConst), CB_TYPE::MATERIAL);

	m_CB[(UINT)CB_TYPE::ANIMATION] = new CConstBuffer;
	m_CB[(UINT)CB_TYPE::ANIMATION]->Create(sizeof(tAnim2DInfo), CB_TYPE::ANIMATION);

	m_CB[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer;
	m_CB[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(tGlobalData), CB_TYPE::GLOBAL);

	return S_OK;
}

int CDevice::CreateSamplerState()
{
	D3D11_SAMPLER_DESC Desc[3] = { };

	// ANISOTROPIC Sampler
	Desc[0].AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[0].AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[0].AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[0].Filter = D3D11_FILTER_ANISOTROPIC;	
	Desc[0].MinLOD = 0;
	Desc[0].MaxLOD = D3D11_FLOAT32_MAX;
	DEVICE->CreateSamplerState(Desc, m_Sampler[0].GetAddressOf());

	CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());


	// MIN_MAG_MIP_POINT Sampler 
	Desc[1].AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[1].AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[1].AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc[1].Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	Desc[1].MinLOD = 0;
	Desc[1].MaxLOD = D3D11_FLOAT32_MAX;
	DEVICE->CreateSamplerState(Desc + 1, m_Sampler[1].GetAddressOf());

	CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->CSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());


	// MIN_MAG_MIP_POINT + CLAMP Sampler 
	Desc[2].AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	Desc[2].AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	Desc[2].AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	Desc[2].Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	Desc[2].MinLOD = 0;
	Desc[2].MaxLOD = D3D11_FLOAT32_MAX;
	DEVICE->CreateSamplerState(Desc + 2, m_Sampler[2].GetAddressOf());

	CONTEXT->VSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
	CONTEXT->HSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
	CONTEXT->DSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
	CONTEXT->GSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
	CONTEXT->PSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
	CONTEXT->CSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());

	return 0;
}

int CDevice::CreateRasterizerState()
{
	// CULL_BACK 기능은 Default 옵션이기 때문에, nullptr 로 둔다.
	m_RS[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	// CULL_FRONT
	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_FRONT;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

	// CULL_NONE
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

	// WIRE_FRAME
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_WIREFRAME;
	DEVICE->CreateRasterizerState(&Desc, m_RS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

	return S_OK;
}

int CDevice::CreateDepthStencilState()
{
	m_DS[(UINT)DS_TYPE::LESS] = nullptr;

	D3D11_DEPTH_STENCIL_DESC Desc = {};


	// LESS_EQUAL
	Desc.DepthEnable = true;	// 깊이 비교 기능 사용
	Desc.StencilEnable = false;	// 스텐실 기능 비활성화
	Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;	// 작거나 같은경우 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 깊이 테스트 성공 시, 깊이 기록

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

	// GREATER
	Desc.DepthEnable = true;	// 깊이 비교 기능 사용
	Desc.StencilEnable = false;	// 스텐실 기능 비활성화
	Desc.DepthFunc = D3D11_COMPARISON_GREATER;	// 작거나 같은경우 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 깊이 테스트 성공 시, 깊이 기록

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::GREATER].GetAddressOf());

	// NO_WRITE
	Desc.DepthEnable = true;	// 깊이 비교 기능 사용
	Desc.StencilEnable = false;	// 스텐실 기능 비활성화
	Desc.DepthFunc = D3D11_COMPARISON_LESS;	// 작은경우 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 깊이 테스트 성공 시, 깊이 기록하지 않음

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());


	// NO_TEST
	Desc.DepthEnable = true;	// 깊이 비교 기능 사용
	Desc.StencilEnable = false;	// 스텐실 기능 비활성화
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;	// 작거나 같은경우 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 깊이 테스트 성공 시, 깊이 기록

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

	// NO_TEST_NO_WRITE
	Desc.DepthEnable = false;	// 깊이 비교 기능 사용
	Desc.StencilEnable = false;	// 스텐실 기능 비활성화
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;	// 작거나 같은경우 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 깊이 테스트 성공 시, 깊이 기록

	DEVICE->CreateDepthStencilState(&Desc, m_DS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());


	// BackFace Check
	Desc.DepthEnable = true;	
	Desc.DepthFunc = D3D11_COMPARISON_GREATER;	// 깊이가 더 멀면 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; 

	Desc.StencilEnable = true;	// 스텐실 기능 비활성화
	Desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 스텐실 검사는 하지 않음
	Desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
	Desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;


	// FrontFace Check
	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_LESS;			// 깊이가 더 가까워야 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	Desc.StencilEnable = true;	// 스텐실 기능 비활성화
	Desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 스텐실 검사는 하지 않음
	Desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
	Desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;


	// BackFace, FrontFace, RS_TYPE::CULL_NONE
	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_GREATER;	// 깊이가 더 멀면 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	Desc.StencilEnable = true;	// 스텐실 기능 비활성화
	Desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 스텐실 검사는 하지 않음
	Desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
	Desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;

	Desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 스텐실 검사는 하지 않음
	Desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	Desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR;



	// Stencil Check
	Desc.DepthEnable = false;							// 깊이 Test 사용 안함
	Desc.DepthFunc = D3D11_COMPARISON_NEVER;			
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;	

	Desc.StencilEnable = true;	// 스텐실 기능 비활성화
	Desc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER; // 특정 스텐실 값을 가진 경우만 통과
	Desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	Desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;



	return S_OK;
}

int CDevice::CreateBlendState()
{
	m_BS[(UINT)BS_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC Desc = {};

	// ALPHA_BLEND
	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;

	// 블랜딩 방식
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	// 블랜딩 계수
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 알파끼리의 혼합식
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	HRESULT hr = DEVICE->CreateBlendState(&Desc, m_BS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());

	// ====================
	// ALPHA_BLEND_COVERAGE
	// ====================
	Desc.AlphaToCoverageEnable = true;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;

	// 블랜딩 방식
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	// 블랜딩 계수
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 알파끼리의 혼합식
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	hr = DEVICE->CreateBlendState(&Desc, m_BS[(UINT)BS_TYPE::ALPHA_BLEND_COVERAGE].GetAddressOf());

	// =======
	// ONE_ONE
	// =======
	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;

	// 블랜딩 방식
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	// 블랜딩 계수
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 알파끼리의 혼합식
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	DEVICE->CreateBlendState(&Desc, m_BS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());


	// ======================================
	// DecalBlend, 0 - AlphaBlend, 1 - OneOne
	// ======================================
	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = true;
	
	// 블랜딩 방식
	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	// 블랜딩 계수
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 알파끼리의 혼합식
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	// 블랜딩 방식
	Desc.RenderTarget[1].BlendEnable = true;
	Desc.RenderTarget[1].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	// 블랜딩 계수
	Desc.RenderTarget[1].SrcBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[1].DestBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 알파끼리의 혼합식
	Desc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;

	DEVICE->CreateBlendState(&Desc, m_BS[(UINT)BS_TYPE::DECAL_BLEND].GetAddressOf());

	return S_OK;
}
