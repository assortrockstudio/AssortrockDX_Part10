#pragma once
#include "CAsset.h"

#include "CSoundMgr.h"

class CSound :
    public CAsset
{
public:
    FMOD::Sound*			m_pSound;       // �ε��� ���� ��ü
    list<FMOD::Channel*>	m_listChannel;  // ���尡 ����ǰ� �ִ� ä��

public:
	// _iRoopCount : 0 (���ѹݺ�),  _fVolume : 0 ~ 1(Volume), _bOverlap : �Ҹ� ��ø ���ɿ���
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
