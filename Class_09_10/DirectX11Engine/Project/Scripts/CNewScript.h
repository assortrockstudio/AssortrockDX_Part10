#pragma once

#include <Engine/CScript.h>

class CNewScript :
    public CScript
{
public:
    virtual void tick() override;


    CLONE(CNewScript);
public:
    CNewScript();
    ~CNewScript();
};

