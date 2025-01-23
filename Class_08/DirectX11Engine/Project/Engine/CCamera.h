#pragma once
#include "CComponent.h"

#include "CFrustum.h"

class CCamera :
    public CComponent
{
private:
    CFrustum                m_Frustum;
    PROJ_TYPE               m_ProjType;
    int                     m_CamPriority;  // 렌더매니저에 등록될때 카메라의 우선순위(0 : 메인카메라, -1 : 미등록 카메라, 0 보다 크다 : 서브 카메라)

    float                   m_FOV;          // 시야각(Field Of View)
    float                   m_Far;          // 시야 최대거리

    float                   m_Width;        // 가로 길이
    float                   m_AspectRatio;  // 종횡 비

    float                   m_Scale;        // 직교투영 배율

    Matrix                  m_matView;
    Matrix                  m_matViewInv;
    Matrix                  m_matProj;
    Matrix                  m_matProjInv;

    UINT                    m_LayerCheck;
    tRay                    m_Ray;


    vector<CGameObject*>    m_vecDeferred;
    vector<CGameObject*>    m_vecDecal;

    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMasked;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecParticle;
    vector<CGameObject*>    m_vecPostProcess;


public:
    void SetCameraPriority(int _Priority);    

    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    void SetFar(float _Far) { m_Far = _Far; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }
    void SetWidth(float _Width) { m_Width = _Width; }
    void SetAspectRaito(float _AR) { m_AspectRatio = _AR; }
	void SetScale(float _Scale) { m_Scale = _Scale; }

	int GetCameraPriority() { return m_CamPriority; }
	PROJ_TYPE GetProjType() { return m_ProjType; }
	float GetFar() { return m_Far; }
	float GetFOV() { return m_FOV; }
	float GetWidth() { return m_Width; }
	float GetAspectRaito() { return m_AspectRatio; }
	float GetScale() { return m_Scale; }
    const tRay& GetRay() { return m_Ray; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetViewInvMat() { return m_matViewInv; }
    const Matrix& GetProjMat() { return m_matProj; }
    const Matrix& GetProjInvMat() { return m_matProjInv; }

    void LayerCheck(int _LayerIdx);
    void LayerCheckAll() { m_LayerCheck = 0xffffffff; }

    void SortObject();

public:
    virtual void begin() override;
    virtual void finaltick() override;
    void render();

private:
    void CalcRay();

public:    
    void SortClear();
    void render_deferred();
    void render_decal();
    void render_opaque();
    void render_masked();
    void render_transparent();
    void render_particle();
    void render_postprocess();
    
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCamera);
public:
    CCamera();
    CCamera(const CCamera& _Other);
    ~CCamera();
};

