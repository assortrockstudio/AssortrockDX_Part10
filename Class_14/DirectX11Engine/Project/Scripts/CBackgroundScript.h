#pragma once
#include <Engine/CScript.h>

class CBackgroundScript :
    public CScript
{
private:
    float   m_Intence;

public:
    virtual void tick() override;

    

public:
    CLONE(CBackgroundScript);
public:
    CBackgroundScript();
    ~CBackgroundScript();
};

