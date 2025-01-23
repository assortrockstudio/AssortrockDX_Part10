#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer*         m_arrLayer[MAX_LAYER];
    LEVEL_STATE     m_State;


public:
    void begin();
    void tick();
    void finaltick();
    void RegisterClear();

public:
    void AddObject(UINT _LayerIdx, CGameObject* _Object, bool _bChildMove = false);
    CLayer* GetLayer(int _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }
    LEVEL_STATE GetState() { return m_State; }

    // 특정 이름을 가진 GameObject 를 검색(제일 먼저 검출된 오브젝트 반환)
    CGameObject* FindObjectByName(const wstring& _Name);

    // 특정 이름을 가진 GameObject 를 모두 찾아서 반환
    void FindObjectByName(const wstring& _Name, vector<CGameObject*>& _vecObj);

    // 특정 컴포넌트를 보유한 GameObject 를 검색(제일 먼저 검출된 오브젝트 반환)
    CGameObject* FindObjectByComponent(COMPONENT_TYPE _Type, const wstring& _Name);

    // 특정 컴포넌트를 가진 GameObject 를 모두 찾아서 반환
    void FindObjectByComponent(COMPONENT_TYPE _Type, vector<CGameObject*>& _vecObj);


private:
    void ChangeState(LEVEL_STATE _NextState);

public:
    CLONE_DISABLE(CLevel);

public:
    CLevel();
    ~CLevel();

    friend class CTaskMgr;
};

