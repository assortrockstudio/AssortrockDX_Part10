#pragma once
#include "CComponent.h"

#include "assets.h"


struct tMtrlSet
{
    Ptr<CMaterial>  pSharedMtrl;    // ���� ���׸���
    Ptr<CMaterial>  pDynamicMtrl;   // ���� ���׸����� ���纻    
    Ptr<CMaterial>  pCurMtrl;       // ���� ��� �� ���׸���
};

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>          m_Mesh;
    vector<tMtrlSet>    m_vecMtrls; // ���� 

    bool                m_FrustumCheck; // Frustum Culling ����    
    bool                m_DynamicShadow; // �����׸��� ���� ����

public:
    virtual void render() = 0;
    virtual void render(UINT _iSubset);
    virtual void render_shadowmap() ;

public:
    void SetMesh(Ptr<CMesh> _Mesh);
    Ptr<CMesh> GetMesh() { return m_Mesh; }

    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx);

    Ptr<CMaterial> GetMaterial(UINT _idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _idx);

    UINT GetMaterialCount() { return (UINT)m_vecMtrls.size(); }

    // ������ �÷��̸�忡���� ��� ����
    Ptr<CMaterial> GetDynamicMaterial(UINT _idx);    
    
    UINT GetMtrlCount() { return (UINT)m_vecMtrls.size(); }

    void SetFrustumCheck(bool _Check) { m_FrustumCheck = _Check; }
    bool IsFrustumCheck() { return m_FrustumCheck; }

    void SetDynamicShadow(bool _Use) { m_DynamicShadow = _Use; }
    bool IsDynamicShadow() { return m_DynamicShadow; }

    ULONG64 GetInstID(UINT _iMtrlIdx);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Other);
    ~CRenderComponent();
};

