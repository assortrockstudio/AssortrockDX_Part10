#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"

#include "components.h"


enum class PROPERTY_TYPE
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    TEXTURE,
};

struct tScriptProperty
{
    PROPERTY_TYPE   Type;
    string          Desc;
    void*           pData;
};


class CScript :
    public CComponent
{
private:
    const int                   m_iScriptType;
    vector<tScriptProperty>     m_vecProperty;

public:
    virtual void tick() = 0;
    virtual void finaltick() final {}
    
 public:
     CGameObject* Instantiate(Ptr<CPrefab> _Prefab, int _LayerIdx, const Vec3& _WorldPos);
     UINT GetScriptType() { return m_iScriptType; }

     const vector<tScriptProperty>& GetScriptProperty() { return m_vecProperty; }

protected:
    void AddScriptProperty(PROPERTY_TYPE _Type, const string& _Desc, void* _pData)
    {
        m_vecProperty.push_back(tScriptProperty{ _Type , _Desc , _pData });
    }


public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}

    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _File) override {};

public:
    CScript(UINT _ScriptType);
    ~CScript();
};

