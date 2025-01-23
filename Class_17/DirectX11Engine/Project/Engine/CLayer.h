#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecParent;    // Layer 소속 오브젝트 중, 최상위 부모만 관리
    vector<CGameObject*>    m_vecObjects;   // Layer 소속 모든 오브젝트들
    int                     m_LayerIdx;

public:
    void begin();
    void tick();
    void finaltick(); 
    
public:
    // _bChildMove : _Object 의 자식들도 해당 레이어로 옮길 것인지 말지
    void AddObject(CGameObject* _Object, bool _bChildMove = false);
   
    void RegisterObject(CGameObject* _Object) { m_vecObjects.push_back(_Object); }
    void Clear() { m_vecObjects.clear(); }

    const vector<CGameObject*>& GetParentObjects() { return m_vecParent; }
    const vector<CGameObject*>& GetObjects() { return m_vecObjects; }

    void DeregisterParentObject(CGameObject* _Object);


    CLONE_DISABLE(CLayer);
public:
    CLayer();
    ~CLayer();

    friend class CLevel;
};

