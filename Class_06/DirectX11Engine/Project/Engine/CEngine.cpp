#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CDbgRenderMgr.h"
#include "CCollisionMgr.h"
#include "CTaskMgr.h"
#include "CSoundMgr.h"
#include "CFontMgr.h"

CEngine::CEngine()
	: m_hMainHwnd(nullptr)
	, m_Resolution{}
{
}

CEngine::~CEngine()
{	
}

int CEngine::init(HWND _hWnd, Vec2 _Resolution, PREFAB_SAVE_FUNC _Save, PREFAB_LOAD_FUNC _Load)
{	
	m_hMainHwnd = _hWnd;
	m_Resolution = _Resolution;

	// 윈도우 크기 설정
	RECT rt = {0, 0, (int)m_Resolution.x, (int)m_Resolution.y};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hMainHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	// SoundMgr 초기화
	CSoundMgr::GetInst()->init();

	// DirectX11
	if (FAILED(CDevice::GetInst()->init(m_hMainHwnd, m_Resolution)))
	{
		MessageBox(m_hMainHwnd, L"Device 초기화 실패", L"엔진 초기화 실패", MB_OK);
		return E_FAIL;
	}	
	
	// Manager Init
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();	
	CAssetMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();
	CFontMgr::GetInst()->init();

	// Prefab Save / Load 함수 설정
	CPrefab::SetPrefabSaveFunc(_Save);
	CPrefab::SetPrefabLoadFunc(_Load);

	return S_OK;
}

void CEngine::progress()
{
	// ============
	// Manager Tick
	// ============
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();
	CSoundMgr::GetInst()->tick();
	CAssetMgr::GetInst()->tick();	

	// Object Tick
	CLevelMgr::GetInst()->tick();
	CCollisionMgr::GetInst()->tick();
	
	// =========
	// Rendering
	// =========
	CRenderMgr::GetInst()->render();	
	CDbgRenderMgr::GetInst()->render();

	CTimeMgr::GetInst()->render();
	CRenderMgr::GetInst()->render_drawcall();	
		
	// Task Execute 
	CTaskMgr::GetInst()->tick();
}

