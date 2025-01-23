#pragma once
#include "CComponent.h"

class CLight2D :
    public CComponent
{
private:
    tLightInfo  m_Info;
    int         m_LightIdx;


public:
    void SetLightType(LIGHT_TYPE _Type);

    void SetDiffuse(Vec3 _Diffuse) { m_Info.Light.vDiffuse = _Diffuse; }
    void SetAmbient(Vec3 _Ambient) { m_Info.Light.vAmbient = _Ambient; }
    void SetSpecular(Vec3 _Spec) { m_Info.Light.vMaxSpecular = _Spec; }

    void SetRange(float _Range) { m_Info.Range = _Range; }
    void SetAngle(float _Angle) { m_Info.Angle = _Angle; }
    


    const tLightInfo& GetLightInfo() { return m_Info; }


public:
    virtual void finaltick() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CLight2D);
public:
    CLight2D();
    ~CLight2D();
};

