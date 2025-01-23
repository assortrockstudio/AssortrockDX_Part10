#pragma once
#include "singleton.h"

union COL_ID
{
    struct 
    {
        UINT LeftID;
        UINT RightID;
    };

    LONGLONG ID;
};



class CCollider2D;

class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
    SINGLE(CCollisionMgr);
private:
    UINT                m_Matrix[(UINT)MAX_LAYER];
    map<LONGLONG, bool> m_ColInfo;


public:
    void tick();

public:
    void LayerCheck(UINT _LayerLeftIdx, UINT _LayerRightIdx);

private:
    void CollisionBtwLayers(UINT _Left, UINT _Right);
    void CollisionBtwCollider2D(CCollider2D* _LeftCol, CCollider2D* _RightCol);
    bool IsCollision(CCollider2D* _LeftCol, CCollider2D* _RightCol);

public:
    void SaveToLevelFile(FILE* _File);
    void LoadFromLevelFile(FILE* _File);
};

