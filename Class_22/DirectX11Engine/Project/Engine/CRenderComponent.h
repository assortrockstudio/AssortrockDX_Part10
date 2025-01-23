#pragma once
#include "CComponent.h"

#include "assets.h"


struct tMtrlSet
{
    Ptr<CMaterial>  pSharedMtrl;    // 공유 메테리얼
    Ptr<CMaterial>  pDynamicMtrl;   // 공유 메테리얼의 복사본    
    Ptr<CMaterial>  pCurMtrl;       // 현재 사용 할 메테리얼
};

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>          m_Mesh;
    vector<tMtrlSet>    m_vecMtrls; // 재질 

    bool                m_FrustumCheck; // Frustum Culling 적용    
    bool                m_DynamicShadow; // 동적그림자 생성 여부

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

    // 레벨이 플레이모드에서만 사용 가능
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

