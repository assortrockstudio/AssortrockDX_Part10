#pragma once
#include "CRenderComponent.h"


class CMeshRender :
    public CRenderComponent
{

public:
    virtual void finaltick() override;
    virtual void render() override;
    
    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();
};

