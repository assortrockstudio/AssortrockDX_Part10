#include "pch.h"
#include "CLevelMgr.h"

#include "CAssetMgr.h"

#include "CLevel.h"
#include "CLayer.h"


CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
	{
		delete m_CurLevel;
	}
}


void CLevelMgr::init()
{
	
}

void CLevelMgr::tick()
{
	if (nullptr != m_CurLevel)
	{
		if (LEVEL_STATE::PLAY == m_CurLevel->GetState())
		{
			m_CurLevel->tick();
		}

		m_CurLevel->RegisterClear();		
		m_CurLevel->finaltick();
	}
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (nullptr != m_CurLevel)
	{
		delete m_CurLevel;
		m_CurLevel = nullptr;
	}

	m_CurLevel = _NextLevel;
}
