#pragma once
#include "CAsset.h"

#include "CSoundMgr.h"

class CSound :
    public CAsset
{
public:
    FMOD::Sound*			m_pSound;       // 로딩된 사운드 객체
    list<FMOD::Channel*>	m_listChannel;  // 사운드가 재생되고 있는 채널

public:
	// _iRoopCount : 0 (무한반복),  _fVolume : 0 ~ 1(Volume), _bOverlap : 소리 중첩 가능여부
	int Play(int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
	void Stop();

	// 0 ~ 1
	void SetVolume(float _f, int _iChannelIdx);

private:
	void RemoveChannel(FMOD::Channel* _pTargetChannel);
	friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void* commanddata1, void* commanddata2);

public:
    virtual int Save(const wstring& _FilePath) override { return S_OK; };
    virtual int Load(const wstring& _FilePath) override;

public:
	CLONE_DISABLE(CSound);

public:
    CSound();
    ~CSound();
};

