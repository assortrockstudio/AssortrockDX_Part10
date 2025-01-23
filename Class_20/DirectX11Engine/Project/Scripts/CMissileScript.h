#pragma once
#include <Engine/CScript.h>

class CMissileScript :
    public CScript
{
private:
    float       m_Speed;

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    CLONE(CMissileScript);
public:
    CMissileScript();
    ~CMissileScript();
};

