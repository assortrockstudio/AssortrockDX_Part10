#pragma once
#include "CEntity.h"

enum FACE_TYPE
{
    FACE_NEAR,
    FACE_FAR,
    FACE_TOP,
    FACE_BOT,
    FACE_LEFT,
    FACE_RIGHT,
};

class CFrustum :
    public CEntity
{
private:
    class CCamera*  m_Camera;
    Vec3            m_arrProjPos[8];
    Vec4            m_arrFace[6];

public:
    void SetOwner(CCamera* _Owner) { m_Camera = _Owner; }
    CCamera* GetOwner() { return m_Camera; }

    bool FrustumPointCheck(Vec3 _WorldPos);
    bool FrustumSphereCheck(Vec3 _WorldPos, float _Radius);

public:
    void finaltick();

public:
    CLONE(CFrustum);
    CFrustum(CCamera* _Cam);
    ~CFrustum();
};

