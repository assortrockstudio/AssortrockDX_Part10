#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CAnimation3DShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_pFrameDataBuffer;			// t16
    CStructuredBuffer* m_pOffsetMatBuffer;			// t17 
    CStructuredBuffer* m_pOutputBuffer;				// u0


public:
    // g_int_0 : BonCount, g_int_1 : Frame Index
    void SetBoneCount(int _iBoneCount) { m_Const.iArr[0] = _iBoneCount; }
    void SetFrameIndex(int _iFrameIdx) { m_Const.iArr[1] = _iFrameIdx; }
    void SetNextFrameIdx(int _iFrameIdx) { m_Const.iArr[2] = _iFrameIdx; }
    void SetFrameRatio(float _fFrameRatio) { m_Const.fArr[0] = _fFrameRatio; }
    void SetFrameDataBuffer(CStructuredBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
    void SetOffsetMatBuffer(CStructuredBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
    void SetOutputBuffer(CStructuredBuffer* _buffer) { m_pOutputBuffer = _buffer; }

public:
    virtual int Binding() override;
    virtual void CalculateGroupNum() override;
    virtual void Clear() override;

public:
    CAnimation3DShader();
    ~CAnimation3DShader();
};

