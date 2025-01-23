#pragma once
#include "singleton.h"

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif


class CSoundMgr :
    public CSingleton<CSoundMgr>
{
    SINGLE(CSoundMgr);
private:
    FMOD::System*   m_pFMODSystem;  // FMOD 관리자 클래스

public:
    FMOD::System* GetFMODSystem() { return m_pFMODSystem; }

public:
    void init();
    void tick();
};

