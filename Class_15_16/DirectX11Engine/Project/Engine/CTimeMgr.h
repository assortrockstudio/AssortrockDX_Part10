#pragma once
#include "singleton.h"


class CTimeMgr :
    public CSingleton<CTimeMgr>
{
    SINGLE(CTimeMgr)
private:
    LARGE_INTEGER   m_llFrequency;  // 초당 카운트 수
    LARGE_INTEGER   m_llCurCount;   // 현재 카운트 수
    LARGE_INTEGER   m_llPrevCount;  // 이전 프레임 카운트 수

    float           m_DT;           // Delta Time : 1프레임 동작하는데 걸린 시간, LevelContent 용도
    float           m_EngineDT;     // Delta Time : 1프레임 동작하는데 걸린 시간

    float           m_Time;         // 프로세스 실행 이후의 누적 시간
    float           m_EngineTime;   // 프로세스 실행 이후의 누적 시간 (레벨 상태에 영향받지 않음)
    float           m_AccTime;      // 1초를 체크하기 위한 시간 누적변수

    UINT            m_FrmCount;
    wchar_t			m_szText[255];

public:
    void init();
    void tick();
    void render();

public:
    float GetDeltaTime() { return m_DT; }
    float GetDeltaTime_Engine() { return m_EngineDT; }
};

