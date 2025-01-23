#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CMesh.h"
#include "CMaterial.h"

class CLight3D :
    public CComponent
{

private:
    tLightInfo      m_Info;
    int             m_LightIdx;

    Ptr<CMesh>      m_VolumeMesh;
    Ptr<CMaterial>  m_LightMtrl;


public:
    void SetLightType(LIGHT_TYPE _Type);

    void SetDiffuse(Vec3 _Diffuse) { m_Info.Light.vDiffuse = _Diffuse; }
    void SetAmbient(Vec3 _Ambient) { m_Info.Light.vAmbient = _Ambient; }
    void SetSpecular(Vec3 _Spec) { m_Info.Light.vMaxSpecular = _Spec; }

    void SetRange(float _Range);
    void SetAngle(float _Angle) { m_Info.Angle = _Angle; }

    const tLightInfo& GetLightInfo() { return m_Info; }

public:
    void Lighting();


public:
    virtual void finaltick() override;
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CLight3D);
public:
    CLight3D();
    ~CLight3D();
};

