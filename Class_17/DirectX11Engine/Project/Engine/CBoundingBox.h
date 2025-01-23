#pragma once
#include "CComponent.h"

class CBoundingBox :
    public CComponent
{
private:
    Vec3    m_OffsetPos;
    float   m_Radius;
    bool    m_Absolute;
    Matrix  m_WorldMat;

public:
    Vec3  GetOffsetPos() { return m_OffsetPos; }
    Vec3 GetWorldPos() { return m_WorldMat.Translation(); }
    float GetRadius() { return m_Radius; }

    void SetOffsetPos(Vec3 _OffsetPos) { m_OffsetPos = _OffsetPos; }
    void SetRaidus(float _Radius) { m_Radius = _Radius; }   

    void SetAbsolute(bool _Abs) { m_Absolute = _Abs; }
    bool IsAbsolute() { return m_Absolute; }

public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _File);

    CLONE(CBoundingBox);
public:
    CBoundingBox();
    ~CBoundingBox();
};

