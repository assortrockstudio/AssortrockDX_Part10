#include "pch.h"
#include "CSoundMgr.h"

#include "CSound.h"

CSoundMgr::CSoundMgr()
	: m_pFMODSystem(nullptr)
{
}

CSoundMgr::~CSoundMgr()
{
	if (nullptr != m_pFMODSystem)
	{
		m_pFMODSystem->release();
		m_pFMODSystem = nullptr;
	}
}

void CSoundMgr::init()
{
	FMOD::System_Create(&m_pFMODSystem);

	if (nullptr == m_pFMODSystem)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	m_pFMODSystem->init(32, FMOD_DEFAULT, nullptr);
}

void CSoundMgr::tick()
{
	m_pFMODSystem->update();
}
