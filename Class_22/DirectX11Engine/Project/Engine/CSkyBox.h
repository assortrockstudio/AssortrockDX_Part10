#pragma once
#include "CRenderComponent.h"

enum SKYBOX_TYPE
{
    SPHERE,
    CUBE,
};

class CSkyBox :
    public CRenderComponent
{
private:
    SKYBOX_TYPE     m_Type;
    Ptr<CTexture>   m_SkyBoxTex;

public:
    void SetSkyBoxType(SKYBOX_TYPE _Type);
    SKYBOX_TYPE GetSkyBoxType() { return m_Type; }

    void SetSkyBoxTexture(Ptr<CTexture> _Texture) { m_SkyBoxTex = _Texture; }
    Ptr<CTexture> GetSkyBoxTexture() { return m_SkyBoxTex; }

public:
    virtual void finaltick() override;
    virtual void render() override;

public:
    CLONE(CSkyBox);
    CSkyBox();
    ~CSkyBox();
};

