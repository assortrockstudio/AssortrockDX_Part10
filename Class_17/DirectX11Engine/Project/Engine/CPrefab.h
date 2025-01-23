#pragma once
#include "CAsset.h"

class CGameObject;

typedef void (*PREFAB_SAVE_FUNC)(CGameObject*, FILE*);
typedef CGameObject* (*PREFAB_LOAD_FUNC)(FILE*);

class CPrefab :
    public CAsset
{
private:    
    static PREFAB_SAVE_FUNC SAVE_FUNC;
    static PREFAB_LOAD_FUNC LOAD_FUNC;
public:
    static void SetPrefabSaveFunc(PREFAB_SAVE_FUNC _Func) { SAVE_FUNC = _Func; }
    static void SetPrefabLoadFunc(PREFAB_LOAD_FUNC _Func) { LOAD_FUNC = _Func; }

private:
    CGameObject*    m_ProtoObj;

public:
    CGameObject* Instantiate();

public:
    virtual int Save(const wstring& _FilePath);
    virtual int Load(const wstring& _FilePath);
        
public:
    CLONE(CPrefab);
    CPrefab(bool _bEngine = false);
    CPrefab(CGameObject* _Proto);
    CPrefab(const CPrefab& _Other);
    ~CPrefab();
};

